/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
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
