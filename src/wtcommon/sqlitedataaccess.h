/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#ifndef SQLITEDATAACCESS_H
#define SQLITEDATAACCESS_H

#include "dataaccess.h"
#include "configuration.h"

#include "sqlite3/sqlite3.h"

#include <functional>
#include <vector>

namespace WT {

class SQLiteDataAccess : public DataAccess
{
    typedef int (*sqlite3_callback)(void *param, int argc, char **argv, char **azColName);

    const std::string table_name = "WT_ENTRIES";

    DataEntry last_entry;
    int saved_entry_counter = 0;
    const int store_cnt;

    std::vector<DataEntry> entries;
    sqlite3 *db = nullptr;
    std::string filename;
    sqlite3_stmt *insert_stmt = nullptr;
    sqlite3_stmt *update_stmt = nullptr;

    DataContainer container;

    static int query_container_callback(void *data_access, int argc, char **argv, char **col_name);
    static std::string translate_operator(char op);
    void load_expression_condition(std::shared_ptr<Operand> op, std::ostream& stream);

    bool table_exists();
    int execute_query(const std::string &sql, sqlite3_callback callback = nullptr);
    void on_database_created();
    void create_database();
    void backup_existing_db();
    void persist_records();
    void prepare_statements();
    void create_regexp_function();
    void init_sqlite3();

public:
    SQLiteDataAccess(const std::string &data_file, const std::shared_ptr<SQLiteDAConfiguration> &configuration);
    virtual ~SQLiteDataAccess();

    void open(bool readonly) override;
    void save_entry(const DataEntry &entry) override;
    DataContainer get_tree(const std::shared_ptr<BinaryExpression>& expression) override;

    static std::string default_data_file();
};

}

#endif // SQLITEDATAACCESS_H
