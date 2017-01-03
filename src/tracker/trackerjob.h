/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef TRACKERJOB_H
#define TRACKERJOB_H

#include "windowinfoprovider.h"
#include "plugincontainer.h"

#include "wtcommon/dataaccess.h"
#include "wtcommon/configuration.h"

#include <condition_variable>
#include <thread>

namespace WT {

class TrackerJob
{
    std::mutex mutex;
    std::condition_variable terminate;
    bool is_running = true;

    std::chrono::seconds period;
    std::shared_ptr<WindowInfoProvider> window_info_provider;
    std::thread th;
    int store_cnt;
    std::shared_ptr<DataAccess> data_access;

    void read_window_info();

    PluginContainer plugin_container;

public:
    TrackerJob(const std::shared_ptr<Configuration>& configuration);

    void run();
    void stop();
};

}

#endif // TRACKERJOB_H
