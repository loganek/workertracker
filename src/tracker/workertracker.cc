/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "workertracker.h"

#include "wtcommon/logger.h"

#include <boost/filesystem.hpp>

#include <iostream>

namespace WT {

WorkerTracker::WorkerTracker()
{
    WT::MethodOutput::set_method([](const std::string& str, LogLevel) {
        std::cout << str << std::flush;
    });
}

int WorkerTracker::pre_process_parameters(int argc, char **argv)
{
    namespace po = boost::program_options;

    std::string config_path;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Print help")
        ("daemon,d", "Run as daemon")
        ("stop,s", "Stop daemon")
        ("config-file,f", po::value<std::string>(&config_path)->default_value(Configuration::get_default_config_path()), "Configuration file")
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

    load_configuration(config_path);

    configuration->add_plugins_path(boost::filesystem::system_complete(boost::filesystem::path(argv[0]).parent_path() / "plugins").string());

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

bool WorkerTracker::lock_app_instance()
{
    auto app_locker = SingleAppLocker::registry();

    if (app_locker)
    {
        if (!app_locker->lock())
        {
            WT_LOG_EMG << "Cannot lock application. Probably one instance of this program is already running on the system!";
            return false;
        }
    }
    else
    {
        WT_LOG_W << "SingleAppLocker not registred in the system!";
    }
    return true;
}

int WorkerTracker::run(int argc, char **argv)
{
    int ret = pre_process_parameters(argc, argv);

    if (ret != 1)
    {
        return ret;
    }

    if (vm.count("daemon"))
    {
        if (auto bg_runner = get_bg_runner())
        {
            bg_runner->register_kill_method([] (int) {
                WT::WorkerTracker::instance().job->stop();
            });

            ret = bg_runner->move_to_background();
        }
    }
    else if (vm.count("stop"))
    {
        if (auto bg_runner = get_bg_runner())
        {
            ret = bg_runner->kill_process();
        }
        else
        {
            ret = -1;
        }
    }

    if (ret != 1)
    {
        return ret;
    }

    if (!lock_app_instance())
    {
        return -1;
    }

    job = std::make_shared<TrackerJob>(configuration);
    job->run();

    return 0;
}

void WorkerTracker::load_configuration(const std::string &config_path)
{
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
