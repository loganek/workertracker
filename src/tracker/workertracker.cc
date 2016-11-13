#include "workertracker.h"
#include "gnomescreensaver.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

#include <iostream>

namespace WT {

WorkerTracker::WorkerTracker()
{
    WT::MethodOutput::set_method(StdOutput::output);

    load_configuration();
}

bool WorkerTracker::process_parameters(int argc, char **argv)
{
    namespace po = boost::program_options;

    int save_period, read_period;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print help")
        ("daemon,d", "Run as daemon")
        ("stop,s", "Stop daemon")
            ("save-period", po::value<int>(&save_period), "Save period (value * read period)")
        ("read-period", po::value<int>(&read_period), "Read period (in seconds)")
    ;

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return false;
    }

    if (vm.count("save-period"))
    {
        configuration->set_general_param("save-period", save_period);
    }
    if (vm.count("read-period"))
    {
        configuration->set_general_param("read-period", read_period);
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

    configuration->print_all_general_params();

    job = std::make_shared<TrackerJob>(configuration);
    job->run();

    return 0;
}

void WorkerTracker::load_configuration()
{
    const char *home_env = std::getenv("HOME");
    boost::filesystem::path config_path(home_env != nullptr ? home_env : "");
    config_path /= std::string(".") + project_name;

    // TODO might throw exception
    if (!boost::filesystem::exists(config_path))
    {
        boost::filesystem::create_directories(config_path);
    }

    config_path /= project_name + std::string(".config");

    WT_LOG_D  << "Load configuration from " << config_path;
    configuration = std::make_shared<WT::Configuration>(config_path.string());
}

}
