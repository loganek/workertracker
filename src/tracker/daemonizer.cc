/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "daemonizer.h"
#include "posixsingleapplocker.h"

#include "wtcommon/unixsyslogloggermethod.h"

#include <csignal>
#include <fstream>
#include <cstring>
#include <unistd.h>

namespace WT {

Daemonizer::RegistrarSingle<Daemonizer> Daemonizer::registrar(std::string(WT_PROJECT_NAME));

Daemonizer::Daemonizer(const std::string &daemon_name)
    : daemon_name(daemon_name)
{
}

int Daemonizer::move_to_background()
{
    pid_t pid;

    WT_LOG_D << "Moving application to background...";

    if ((pid = fork()) < 0)
    {
        WT_LOG_EMG << "forking child process failed!";
        return -1;
    }

    if (pid > 0)
    {
        WT_LOG_D << "Application moved to background! PID: " << pid;
        return 0;
    }

    WT_LOG_D << "Application in background";

    if (setsid() < 0)
    {
        WT_LOG_EMG << "Cannot create session!";
        return -1;
    }

    if ((chdir("/")) < 0)
    {
        WT_LOG_EMG << "Cannot change directory to root '/'";
        return -1;
    }

    UNIXSysLogLoggerMethod::init_log(daemon_name);
    MethodOutput::set_method(UNIXSysLogLoggerMethod::output);

    return 1;
}

int Daemonizer::kill_process()
{
    auto pid_file_name = PosixSingleAppLocker::get_lock_file();
    std::ifstream pid_file(pid_file_name);

    if (!pid_file.is_open())
    {
        WT_LOG_ERR << "Could not open PID lock file " << pid_file_name;
        return -1;
    }

    pid_t pid_value;
    pid_file >> pid_value;
    pid_file.close();

    WT_LOG_I << "Sending signal to process " << pid_value << "...";
    if (kill(pid_value, SIGINT) < 0)
    {
        WT_LOG_ERR << "Sending signal failed: " << std::strerror(errno);
        return -1;
    }
    WT_LOG_I << "Sending signal successfully completed!";

    return 0;
}

void Daemonizer::register_kill_method(void(*handler)(int))
{
    std::signal(SIGTERM, handler);
    std::signal(SIGINT, handler);
    std::signal(SIGHUP, handler);
}

}
