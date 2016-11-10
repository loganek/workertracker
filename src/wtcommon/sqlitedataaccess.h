#ifndef SQLITEDATAACCESS_H
#define SQLITEDATAACCESS_H

#include "dataaccess.h"

#include <sqlite3.h>

#include <functional>
#include <vector>

namespace WT {

class SQLiteDataAccess : public DataAccess
{
    typedef int (*sqlite3_callback)(void *param, int argc, char **argv, char **azColName);

    const std::string table_name = "WT_ENTRIES";

    DataEntry last_entry;

    std::vector<DataEntry> entries;
    sqlite3 *db = nullptr;
    std::string filename;
    bool create_db;

    DataContainer container;

    static int db_created_callback(void *data_access, int argc, char **argv, char **col_name);
    static int query_container_callback(void *data_access, int argc, char **argv, char **col_name);

    int execute_query(const std::string &sql, sqlite3_callback callback = nullptr);
    void on_database_created();
    void create_database();

public:
    SQLiteDataAccess(const std::string &filename);

    void open(bool readonly) override;
    void save_entry(const DataEntry &entry) override;
    void persist_records() override;
    DataContainer get_tree(DataPeriod period = DataPeriod()) override;

    virtual ~SQLiteDataAccess();
};

}

#endif // SQLITEDATAACCESS_H
