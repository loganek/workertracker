/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */

#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <string>
#include <ctime>

namespace WT {

struct DataEntry
{
    std::time_t time_start = 0;
    std::time_t time_end = 0;
    std::string proc_name;
    std::string description;

    DataEntry() {}
    DataEntry(const std::string &proc_name, const std::string& description, std::time_t time_start, std::time_t time_end)
        : time_start(time_start), time_end(time_end), proc_name(proc_name), description(description)
    {}
};

}

#endif // DATAENTRY_H
