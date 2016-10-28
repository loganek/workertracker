#include "workertracker.h"
#include "gnomescreensaver.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

#include <iostream>

namespace WT {

WorkerTracker::WorkerTracker()
{
    WT::MethodOutput::set_method(StdOutput::output);
}

bool WorkerTracker::process_parameters(int argc, char **argv)
{
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print help")
        ("daemon,d", "Run as daemon")
        ("stop,s", "Stop daemon")
        ("save-period", po::value<int>(&save_period)->default_value(save_period_default), "Save period (value * read period)")
        ("read-period", po::value<int>(&read_period)->default_value(read_period_default), "Read period (in seconds)")
    ;

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return false;
    }

    return true;
}

int WorkerTracker::run(int argc, char **argv)
{
    if (!process_parameters(argc, argv))
    {
        return 0;
    }

    bg_runner = BackgroundRunner::registry();

    if (!bg_runner)
    {
        WT_LOG(LogLevel::EMERGENCY) << "Background runner has not been registered in the system!";
        return -1;
    }

    int ret = 1;

    if (vm.count("daemon"))
    {
        bg_runner->register_kill_method([] (int) {
            WT::WorkerTracker::instance().job->stop();
        });

        MethodOutput::set_method(SysLogOutput::output);
        ret = bg_runner->move_to_background();
    }

    if (vm.count("stop"))
    {
        ret = bg_runner->kill_process();
    }

    if (ret != 1)
    {
        return ret;
    }

    const char *home_env = std::getenv("HOME");
    boost::filesystem::path data_path(home_env != nullptr ? home_env : "");
    data_path /= std::string(".") + project_name;

    try {
        boost::filesystem::create_directories(data_path);
    } catch (const boost::filesystem::filesystem_error &ex)
    {
        WT_LOG(LogLevel::ERROR) << ex.what();
        return -1;
    }

    data_path /= "data.dat";

    WT_LOG(LogLevel::INFO) << "Data path: " << data_path;

    job = std::make_shared<TrackerJob>(std::chrono::seconds(read_period), save_period, data_path.string());
    job->run();

    return 0;
}

}
