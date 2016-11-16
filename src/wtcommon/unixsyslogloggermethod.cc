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
