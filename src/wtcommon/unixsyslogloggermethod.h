#ifndef UNIXSYSLOGLOGGERMETHOD_H
#define UNIXSYSLOGLOGGERMETHOD_H

#include "logger.h"

namespace WT {

class UNIXSysLogLoggerMethod
{
public:
    static void output(const std::string& msg, LogLevel level);
    static void init_log(const std::string &daemon_name);
};

}

#endif // UNIXSYSLOGLOGGERMETHOD_H
