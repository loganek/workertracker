#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include "datastorage.h"

#include <string>

namespace WT {

class FileStorage : DataStorage
{
    std::string filename;

    void create_file();

public:
    FileStorage(const std::string &filename);
    virtual ~FileStorage();

    void load_data(DataContainer &container);
    void save_data(const DataContainer &container);
};

}

#endif // FILESTORAGE_H
