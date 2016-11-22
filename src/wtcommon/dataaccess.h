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
    virtual void persist_records() = 0;
    virtual DataContainer get_tree(DateRange period = DateRange()) = 0;
};

}

#endif // DATAACCESS_H
