/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "fileloggermethod.h"

#include <iostream>

namespace WT {

std::ostream* FileLoggerMethod::out_file = nullptr;

void FileLoggerMethod::output(const std::string &str, LogLevel)
{
    if (out_file)
    {
        *out_file << str << std::flush;
    }
}

void FileLoggerMethod::init_log(const std::string &filename)
{
    static std::ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::app);

    if (!file.is_open())
    {
        WT_LOG_ERR << "Can't open file: " << filename << ", use std::cout stream";
        out_file = &std::cout;
    }
    else
    {
        out_file = &file;
    }
}

}
