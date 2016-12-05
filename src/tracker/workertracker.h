/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef WORKERTRACKER_H
#define WORKERTRACKER_H

#include "trackerjob.h"
#include "backgroundrunner.h"
#include "singleapplocker.h"

#include "wtcommon/configuration.h"

#include <boost/program_options.hpp>

namespace WT {

class WorkerTracker
{
    std::shared_ptr<TrackerJob> job;
    std::shared_ptr<Configuration> configuration;
    std::string pid_file_name;
    boost::program_options::variables_map vm;

    int pre_process_parameters(int argc, char **argv);
    void load_configuration(const std::string &config_path);

    static std::shared_ptr<BackgroundRunner> get_bg_runner();
    bool lock_app_instance();

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
