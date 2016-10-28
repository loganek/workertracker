#ifndef WORKERTRACKER_H
#define WORKERTRACKER_H

#include "trackerjob.h"
#include "backgroundrunner.h"

#include <boost/program_options.hpp>

namespace WT {

class WorkerTracker
{
    const char* const project_name = "workertracker";

    std::shared_ptr<TrackerJob> job;
    std::shared_ptr<BackgroundRunner> bg_runner;

    const int save_period_default = 12;
    const int read_period_default = 5;

    std::string pid_file_name;
    int save_period;
    int read_period;

    boost::program_options::variables_map vm;

    bool process_parameters(int argc, char **argv);

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
