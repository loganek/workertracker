#include "trackerjob.h"
#include "gnomescreensaver.h"

#include "wtcommon/logger.h"
#include "wtcommon/sqlitedataaccess.h"

#include <future>

namespace WT {

TrackerJob::TrackerJob(const std::shared_ptr<Configuration>& configuration)
    : period(configuration->get_general_param<int>("read-period").get()),
      store_cnt(configuration->get_general_param<int>("save-period").get()),
      suspendable(configuration)
{
    window_info_provider = WindowInfoProvider::registry();

    if (!window_info_provider)
    {
        WT_LOG(LogLevel::EMERGENCY) << "Window info provider not found!";
        stop();
    }

    data_access = std::make_shared<SQLiteDataAccess>(configuration->get_general_param("data-path").get());

    data_access->open(false);
}

void TrackerJob::read_window_info()
{
    int counter = 0;
    auto locked = std::unique_lock<std::mutex>(mutex);
    entry.time_end = std::time(nullptr);

    std::future<void> persist_result;

    while (is_running)
    {
        auto window_info = window_info_provider->get_current_window_info();

        entry.description = window_info.window_title;
        entry.proc_name = window_info.app_name;
        entry.time_start = entry.time_end;
        terminate.wait_for(locked, period);
        entry.time_end = std::time(nullptr);

        // TODO: it's a hack. I need to figure out better way than this.
        // And the magic number...
        if (entry.time_end - entry.time_start > period.count() * 3)
        {
            WT_LOG_D << "Logging canceled. Most likely computer has been suspended during the waiting period.";
            continue;
        }

        if (suspendable.suspend_tracking(window_info))
        {
            WT_LOG_D << "Logging suspended";
            continue;
        }

        WT_LOG_D << "Saving entry: {" << entry.proc_name << ", " << entry.description << ", " << (entry.time_end - entry.time_start) << "}";

        if (persist_result.valid())
        {
            persist_result.get();
        }

        persist_result = std::future<void>(std::async([this, &counter] {
            data_access->save_entry(entry);

            // TODO move this to data access, so the interface will have only
            // save_entry() method.
            if (++counter % store_cnt == 0)
            {
                WT_LOG_D << "Persisiting records";
                data_access->persist_records();
                counter = 0;
            }
        }));
    }
}

void TrackerJob::stop()
{
    {
        auto locked = std::unique_lock<std::mutex>(mutex);
        is_running = false;
    }

    terminate.notify_one();
 }

void TrackerJob::run()
{
    th = std::thread(&TrackerJob::read_window_info, this);

    if (th.joinable())
    {
        th.join();
    }

    data_access->persist_records();
}

}
