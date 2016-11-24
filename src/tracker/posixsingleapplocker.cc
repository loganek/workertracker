/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "posixsingleapplocker.h"

#include "wtcommon/logger.h"

#include <fcntl.h>
#include <unistd.h>

namespace WT {

PosixSingleAppLocker::RegistrarSingle<PosixSingleAppLocker> PosixSingleAppLocker::registrar;

bool PosixSingleAppLocker::lock()
{
    std::string pid_file_name = get_lock_file();
    pid_file_handle = open(pid_file_name.c_str(), O_RDWR|O_CREAT, 0600);

    if (pid_file_handle == -1)
    {
        WT_LOG_ERR << "Could not open PID lock file " << pid_file_name;
        WT_LOG_ERR << "Probably one instance of application is already running, exiting.";
        return false;
    }

    if (lockf(pid_file_handle, F_TLOCK, 0) == -1)
    {
        WT_LOG_W << "Could not lock PID lock file " << pid_file_name << ", exiting";
        unlock();
        return false;
    }

    std::string pid_s = std::to_string(getpid());
    write(pid_file_handle, pid_s.c_str(), pid_s.length());

    return true;
}

void PosixSingleAppLocker::unlock()
{
    if (pid_file_handle != -1)
    {
        close(pid_file_handle);
        pid_file_handle = -1;
    }
}

std::string PosixSingleAppLocker::get_lock_file()
{
    static std::string lock_file = "/tmp/lock-" + std::string(WT_PROJECT_NAME) + ".pid";
    return lock_file;
}

}
