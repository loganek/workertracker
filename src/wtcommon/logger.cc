#include "logger.h"

#include <syslog.h>

#include <iostream>
#include <chrono>

namespace WT {

std::string to_string(LogLevel level)
{
    static const char* const buffer[] = {"EMERGENCY", "ERROR", "WARNING", "INFO", "DEBUG"};
    return buffer[static_cast<std::underlying_type_t<LogLevel>>(level)];
}

std::ostringstream& prepare_stream(std::ostringstream& os, LogLevel level)
{
    auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto now_tm = std::localtime(&now_time);

    os << "- " << now_tm->tm_hour << ":" << now_tm->tm_min << ":" << now_tm->tm_sec;
    os << " " << to_string(level) << ": ";

    return os;
}

void MethodOutput::output(const std::string& msg, LogLevel level)
{
    out_method(msg, level);
}

MethodOutput::method_t MethodOutput::out_method = [](const std::string& str, LogLevel) { std::cout << str << std::flush; };

void MethodOutput::set_method(method_t method)
{
    out_method = method;
}

void SysLogOutput::output(const std::string &str, LogLevel level)
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

void SysLogOutput::init_log(const std::string &daemon_name)
{
    openlog(daemon_name.c_str(), LOG_CONS | LOG_PERROR, LOG_USER);
}

}
