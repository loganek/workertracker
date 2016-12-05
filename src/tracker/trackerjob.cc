/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "trackerjob.h"

#include "wtcommon/logger.h"
#include "wtcommon/sqlitedataaccess.h"

#include <future>

namespace WT {

TrackerJob::TrackerJob(const std::shared_ptr<Configuration>& configuration)
    : period(configuration->get_general_param<int>("read-period").get()),
      suspendable(configuration)
{
    data_access = std::make_shared<SQLiteDataAccess>(SQLiteDataAccess::default_data_file(), configuration);
    data_access->open(false);

    window_info_provider = WindowInfoProvider::registry();

    if (!window_info_provider)
    {
        WT_LOG_EMG << "Window info provider not found!";
        stop();
    }

    if (!window_info_provider->initialize(configuration))
    {
        WT_LOG_EMG << "Window info provider initialization failed!";
        stop();
    }
}

void TrackerJob::read_window_info()
{
    auto locked = std::unique_lock<std::mutex>(mutex);

    DataEntry entry;
    entry.time_end = std::time(nullptr);

    std::future<void> persist_result;

    while (is_running)
    {
        auto window_info = window_info_provider->get_current_window_info();

        entry.description = window_info.get_window_title();
        entry.proc_name = window_info.get_app_name();
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

        persist_result = std::future<void>(std::async([this, entry] {
            data_access->save_entry(entry);
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
}

}
