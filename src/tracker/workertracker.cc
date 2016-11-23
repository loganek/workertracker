#include "workertracker.h"
#include "gnomescreensaver.h"

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

std::shared_ptr<BackgroundRunner> WorkerTracker::get_bg_runner()
{
    auto bg_runner = BackgroundRunner::registry();

    if (!bg_runner)
    {
        WT_LOG(LogLevel::WARNING) << "Background runner has not been registered in the system!";
    }

    return bg_runner;
}

int WorkerTracker::run(int argc, char **argv)
{
    if (!process_parameters(argc, argv))
    {
        return 0;
    }

    int ret = 1;

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
    else if (vm.count("stop")) // TODO enum parameter in boost::optional? daemon|stop
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

    job = std::make_shared<TrackerJob>(configuration);
    job->run();

    return 0;
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
