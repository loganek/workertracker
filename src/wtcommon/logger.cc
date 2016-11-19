#include "logger.h"

#include <ctime>
#include <iostream>
#include <chrono>
#include <iomanip>

namespace WT {

static std::string to_string(LogLevel level)
{
    static const char* const buffer[] = {"EMERGENCY", "ERROR", "WARNING", "INFO", "DEBUG"};
    return buffer[static_cast<std::underlying_type_t<LogLevel>>(level)];
}

std::ostringstream& prepare_stream(std::ostringstream& os, LogLevel level)
{
    auto now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto now_tm = std::localtime(&now_time);

    os << "- " << std::setfill('0') << std::setw(2) << now_tm->tm_hour
       << ":" << std::setfill('0') << std::setw(2) << now_tm->tm_min
       << ":" << std::setfill('0') << std::setw(2) << now_tm->tm_sec;
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

}
