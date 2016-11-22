#ifndef WORKERTRACKER_H
#define WORKERTRACKER_H

#include "trackerjob.h"
#include "backgroundrunner.h"

#include "wtcommon/configuration.h"

#include <boost/program_options.hpp>

namespace WT {

class WorkerTracker
{
    std::shared_ptr<TrackerJob> job;
    std::shared_ptr<Configuration> configuration;
    std::string pid_file_name;
    boost::program_options::variables_map vm;

    bool process_parameters(int argc, char **argv);
    void load_configuration();

    static std::shared_ptr<BackgroundRunner> get_bg_runner();

    WorkerTracker(WorkerTracker const&) = delete;
    WorkerTracker(WorkerTracker&&) = delete;
    WorkerTracker& operator=(WorkerTracker const&) = delete;
    WorkerTracker& operator=(WorkerTracker &&) = delete;

protected:
    WorkerTracker();

public:
    int run(int argc, char **argv);

    static WorkerTracker& instance()
    {
        static WorkerTracker instance;
        return instance;
    }
};

}

#endif // WORKERTRACKER_H
