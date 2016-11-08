#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <ctime>
#include <string>

namespace WT {

struct DataEntry
{
    std::time_t time;
    std::string proc_name;
    std::string description;
};

class DataAccess
{
public:
    virtual ~DataAccess() {}

    virtual void open() = 0;
    virtual void save_entry(const DataEntry &entry) = 0;
    virtual void persist_records() = 0;
};

}

#endif // DATAACCESS_H
