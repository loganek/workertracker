/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "workertracker.h"
#include "singleapplocker.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

#include <iostream>

namespace WT {

WorkerTracker::WorkerTracker()
{
    WT::MethodOutput::set_method([](const std::string& str, LogLevel) {
        std::cout << str << std::flush;
    });

    load_configuration();
}

int WorkerTracker::pre_process_parameters(int argc, char **argv)
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
        return 0;
    }

    if (vm.count("daemon") && vm.count("stop"))
    {
        WT_LOG_EMG << "Can't use 'daemon' and 'stop' parameters at the same time!";
        return -1;
    }

    if (vm.count("save-period"))
    {
        configuration->set_general_param("save-period", save_period);
    }
    if (vm.count("read-period"))
    {
        configuration->set_general_param("read-period", read_period);
    }

    configuration->set_general_param("current-program-path", std::string(argv[0]));

    return 1;
}

std::shared_ptr<BackgroundRunner> WorkerTracker::get_bg_runner()
{
    auto bg_runner = BackgroundRunner::registry();

    if (!bg_runner)
    {
        WT_LOG_W << "Background runner has not been registered in the system!";
    }

    return bg_runner;
}

int WorkerTracker::run(int argc, char **argv)
{
    int ret = pre_process_parameters(argc, argv);

    if (ret != 1)
    {
        return ret;
    }

    if (vm.count("stop"))
    {
        if (auto bg_runner = get_bg_runner())
        {
            return bg_runner->kill_process();
        }
        else
        {
            return -1;
        }
    }

    auto run_callback = [this] {
        if (!SingleAppLocker::lock_app_instance()) // Daemon is responsible for locking the application
        {
            return -1;
        }

        job = std::make_shared<TrackerJob>(configuration);
        job->run();

        return 0;
    };

    if (vm.count("daemon"))
    {
        if (auto bg_runner = get_bg_runner())
        {
            bg_runner->register_kill_method([] (int) {
                WT::WorkerTracker::instance().job->stop();
            });
            return bg_runner->run_in_background(run_callback);
        }
        else
        {
            WT_LOG_ERR << "Cannot run application in background";
        }
    }

    return run_callback();
}

void WorkerTracker::load_configuration()
{
    std::string config_path = Configuration::get_default_config_path();

    try
    {
        configuration = std::make_shared<WT::Configuration>(config_path);
    }
    catch (const std::exception& ex)
    {
        WT_LOG_ERR << "Can't load configuration: " << config_path << ": " << ex.what();
    }
}

}
