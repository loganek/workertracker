/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef DATAACCESS_H
#define DATAACCESS_H

#include "datacontainer.h"

#include <ctime>
#include <string>

namespace WT {

struct DataEntry
{
    std::time_t time_start = 0;
    std::time_t time_end = 0;
    std::string proc_name;
    std::string description;

    DataEntry() {}
    DataEntry(std::time_t time_start, std::time_t time_end, const std::string &proc_name, const std::string &description)
        : time_start(time_start), time_end(time_end), proc_name(proc_name), description(description)
    {}
};

struct DateRange
{
    std::time_t from = 0;
    std::time_t to = 0;
};

class DataAccess
{
public:
    virtual ~DataAccess() {}

    virtual void open(bool readonly) = 0;
    virtual void save_entry(const DataEntry &entry) = 0;
    virtual DataContainer get_tree(DateRange period = DateRange()) = 0;
    virtual std::vector<DataEntry> get_entries(DateRange period = DateRange()) = 0;
};

}

#endif // DATAACCESS_H
