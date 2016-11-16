#include "daemonizer.h"

#include "wtconstants.h"
#include "wtcommon/logger.h"

#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <fstream>
#include <cstring>

namespace WT {

Daemonizer::RegistrarSingle<Daemonizer> Daemonizer::registrar(std::string(WT_PROJECT_NAME));

Daemonizer::Daemonizer(const std::string &daemon_name)
    : daemon_name(daemon_name),
      pid_file_name("/tmp/daemon-" + std::string(daemon_name) + ".pid")
{
}

Daemonizer::~Daemonizer()
{
    close(pid_file_handle);
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

    SysLogOutput::init_log(daemon_name);
    MethodOutput::set_method(SysLogOutput::output);

    pid_file_handle = open(pid_file_name.c_str(), O_RDWR|O_CREAT, 0600);

    if (pid_file_handle == -1)
    {
        WT_LOG_ERR << "Could not open PID lock file " << pid_file_name << ", exiting.";
        return -1;
    }

    if (lockf(pid_file_handle, F_TLOCK, 0) == -1)
    {
        WT_LOG_W << "Could not lock PID lock file " << pid_file_name << ", exiting";
        return -1;
    }

    std::string pid_s = std::to_string(getpid());
    write(pid_file_handle, pid_s.c_str(), pid_s.length());

    return 1;
}

int Daemonizer::kill_process()
{
    std::ifstream pid_file(pid_file_name);

    if (!pid_file.is_open())
    {
        WT_LOG(LogLevel::ERROR) << "Could not open PID lock file " << pid_file_name;
        return -1;
    }

    pid_t pid_value;
    pid_file >> pid_value;
    pid_file.close();

    WT_LOG(LogLevel::INFO) << "Sending signal to process " << pid_value << "...";
    if (kill(pid_value, SIGINT) < 0)
    {
        WT_LOG(LogLevel::ERROR) << "Sending signal failed: " << std::strerror(errno);
        return -1;
    }
    WT_LOG(LogLevel::INFO) << "Sending signal successfully completed!";
    return 0;
}

void Daemonizer::register_kill_method(void(*handler)(int))
{
    std::signal(SIGTERM, handler);
    std::signal(SIGINT, handler);
    std::signal(SIGHUP, handler);
}

}
