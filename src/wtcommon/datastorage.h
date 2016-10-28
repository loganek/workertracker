#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "datacontainer.h"

namespace WT {

class DataStorage
{
public:
    virtual ~DataStorage() {}

    virtual void load_data(DataContainer &container) = 0;
    virtual void save_data(const DataContainer &container) = 0;
};

}

#endif // DATASTORAGE_H
