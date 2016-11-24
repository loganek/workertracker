/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "unixsyslogloggermethod.h"

#include <syslog.h>

namespace WT {

void UNIXSysLogLoggerMethod::output(const std::string &str, LogLevel level)
{
    int sys_level;
    switch (level)
    {
    case LogLevel::EMERGENCY:
        sys_level = LOG_EMERG;
        break;
    case LogLevel::ERROR:
        sys_level = LOG_ERR;
        break;
    case LogLevel::WARNING:
        sys_level = LOG_WARNING;
        break;
    case LogLevel::INFO:
        sys_level = LOG_INFO;
        break;
    default:
        sys_level = LOG_DEBUG;
        break;
    }

    syslog(sys_level, "%s", str.c_str());
}

void UNIXSysLogLoggerMethod::init_log(const std::string &daemon_name)
{
    openlog(daemon_name.c_str(), LOG_CONS | LOG_PERROR, LOG_USER);
}

}
