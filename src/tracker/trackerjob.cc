#include "trackerjob.h"
#include "wtcommon/logger.h"
#include "gnomescreensaver.h"
namespace WT {

TrackerJob::TrackerJob(std::chrono::seconds read_interval, int save_interval, const std::string &filename)
    : period(read_interval),
      store_cnt(save_interval)
{
    window_info_provider = WindowInfoProvider::registry();

    if (!window_info_provider)
    {
        WT_LOG(LogLevel::EMERGENCY) << "Window info provider not found!";
        stop();
    }

    storage = std::make_shared<FileStorage>(filename);

    storage->load_data(container);
}

void TrackerJob::read_window_info()
{
    int counter = 0;
    auto locked = std::unique_lock<std::mutex>(mutex);

    while (is_running)
    {
        terminate.wait_for(locked, period);

        if (is_suspended())
        {
            continue;
        }

        auto window_info = window_info_provider->get_current_window_info();

        container.insert(window_info.app_name, window_info.window_title, period);

        if (++counter % store_cnt == 0)
        {
            storage->save_data(container);
            counter = 0;
        }
    }
}

bool TrackerJob::is_suspended()
{
    for (const auto& susp : ITrackSuspendable::registry())
    {
        if (susp.second->suspend_tracking())
        {
            return true;
        }
    }

    return false;
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

    storage->save_data(container);
}

}
