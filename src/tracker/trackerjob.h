#ifndef TRACKERJOB_H
#define TRACKERJOB_H

#include "windowinfoprovider.h"
#include "wtcommon/itracksuspendable.h"
#include "suspendablecontainer.h"

#include "wtcommon/dataaccess.h"
#include "wtcommon/datacontainer.h"
#include "wtcommon/filestorage.h"

#include <csignal>
#include <condition_variable>
#include <thread>

namespace WT {

class TrackerJob
{
    std::mutex mutex;
    std::condition_variable terminate;
    bool is_running = true;

    DataEntry entry;

    std::chrono::seconds period;
    std::shared_ptr<WindowInfoProvider> window_info_provider;
    std::thread th;
    DataContainer container;
    int store_cnt;
    std::shared_ptr<DataAccess> data_access;

    void read_window_info();

    SuspendableContainer suspendable;

public:
    TrackerJob(std::chrono::seconds read_interval, int save_interval, const std::string &filename);

    void run();
    void stop();
};

}

#endif // TRACKERJOB_H
