/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <marcin.kolny@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.       Marcin Kolny
 * ----------------------------------------------------------------------------
 */
#include "sqlitedataaccess.h"
#include "logger.h"

#include <boost/filesystem.hpp>

namespace WT {

SQLiteDataAccess::SQLiteDataAccess(const std::shared_ptr<Configuration> &configuration)
    : store_cnt(std::stoi(configuration->get_general_param("save-period").get())),
      filename(configuration->get_general_param("data-path").get())
{
}

int SQLiteDataAccess::query_container_callback(void *data_access, int argc, char **argv, char ** /*col_name*/)
{
    SQLiteDataAccess* this_ = reinterpret_cast<SQLiteDataAccess*>(data_access);

    for (int i = 0; i < argc; i += 3)
    {
        this_->container.insert(argv[i], argv[i+1], std::chrono::seconds(std::stoll(argv[i+2])));
    }

   return 0;
}

bool SQLiteDataAccess::table_exists()
{
    auto sql = "SELECT name FROM sqlite_master WHERE name='" + table_name + "'";
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        WT_LOG_ERR << "Can't prepare sql statement";
        return false;
    }

    bool table_exists = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    WT_LOG_D << "Does table " << table_name << " exists? : " << table_exists;
    return true;
}

void SQLiteDataAccess::prepare_statements()
{
    auto insert_sql = "INSERT INTO " + table_name + "(TIME_START, TIME_END, PROC_NAME, DESCRIPTION) VALUES(?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &insert_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Can't prepare insert statement");
    }
    WT_LOG_D << sqlite3_bind_parameter_count(insert_stmt);

    auto update_sql = "UPDATE " + table_name + " set TIME_END = ? WHERE TIME_END = ?;";
    if (sqlite3_prepare_v2(db, update_sql.c_str(), -1, &update_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Can't prepare update statement");
    }
}

void SQLiteDataAccess::open(bool readonly)
{
    if (boost::filesystem::exists(filename))
    {
        int open_flags = readonly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (sqlite3_open_v2(filename.c_str(), &db, open_flags, nullptr) != SQLITE_OK)
        {
            WT_LOG_W << "unable to open database: " << sqlite3_errmsg(db);
        }
        else if (table_exists())
        {
            prepare_statements();
            return;
        }

        sqlite3_close(db);
        db = nullptr;
    }

    if (readonly)
    {
        throw std::runtime_error("Cannot open database!");
    }
    else
    {
        create_database();
    }
    prepare_statements();
}

SQLiteDataAccess::~SQLiteDataAccess()
{
    if (db)
    {
        persist_records();
        sqlite3_finalize(insert_stmt);
        sqlite3_finalize(update_stmt);
        sqlite3_close_v2(db);
    }
}

int SQLiteDataAccess::execute_query(const std::string &sql, sqlite3_callback callback)
{
    char *err_msg = nullptr;

    WT_LOG_D << "Executing query " << sql;
    int rc = sqlite3_exec(db, sql.c_str(), callback, this, &err_msg);

    if (rc != SQLITE_OK)
    {
        WT_LOG_ERR << "Cannot execute query " << sql << ": " << err_msg;
        sqlite3_free(err_msg);
    }

    return rc;
}

void SQLiteDataAccess::create_database()
{
    WT_LOG_I << "Database doesn't exist. Create database...";

    if (boost::filesystem::exists(filename))
    {
        backup_existing_db();
    }
    else
    {
        boost::filesystem::create_directories(boost::filesystem::path(filename).parent_path());
    }

    if (sqlite3_open_v2(filename.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Unable to open database: " + std::string(sqlite3_errmsg(db)));
    }

    auto sql = "CREATE TABLE " + table_name + "(" \
                      "TIME_START   INTEGER PRIMARY KEY  NOT NULL," \
                      "TIME_END     INTEGER," \
                      "PROC_NAME    CHAR(50)," \
                      "DESCRIPTION  CHAR(150));";

    if (execute_query(sql) != SQLITE_OK)
    {
        throw std::runtime_error("Cannot create database!");
    }

    WT_LOG_I << "Database succesfully created: " << filename;
}

void SQLiteDataAccess::backup_existing_db()
{
    auto rawtime = std::time(nullptr);
    auto timeinfo = std::localtime(&rawtime);
    char buf[32];
    strftime(buf, 32, ".%F_%H-%M-%S.bak", timeinfo);
    WT_LOG_I << "Move file from " << filename << " to " << filename + buf;
    boost::filesystem::rename(filename, filename + buf);
}

static bool is_continuous_entry(const DataEntry& before, const DataEntry& after)
{
    return before.description == after.description
            && before.proc_name == after.proc_name
            && before.time_end == after.time_start;
}

void SQLiteDataAccess::save_entry(const DataEntry &entry)
{
    if (!entries.empty() && is_continuous_entry(entries.back(), entry))
    {
        entries.back().time_end = entry.time_end;
    }
    else
    {
        entries.push_back(entry);
    }

    if (++saved_entry_counter % store_cnt == 0)
    {
        persist_records();
        saved_entry_counter = 0;
    }
}

static void execute_statement(bool binding_result, sqlite3_stmt *statement)
{
    if (!binding_result)
    {
        WT_LOG_ERR << "Can't bind parameters for update!";
    }

    auto status = sqlite3_step(statement);
    if (status != SQLITE_DONE)
    {
        WT_LOG_D << "Unexpected status of sqlite3_step(): " << status;
    }
    sqlite3_clear_bindings(statement);
    sqlite3_reset(statement);
}

void SQLiteDataAccess::persist_records()
{
    if (entries.empty())
    {
        WT_LOG_D << "Nothing to persist";
        return;
    }

    WT_LOG_D << "Persisiting records";

    std::size_t i = 0;

    if (is_continuous_entry(last_entry, entries.front()))
    {
        WT_LOG_D << "Updating entry";
        bool ret = sqlite3_bind_int64(update_stmt, 1, entries.front().time_end) == SQLITE_OK;
        ret &= sqlite3_bind_int64(update_stmt, 2, entries.front().time_start) == SQLITE_OK;

        execute_statement(ret, update_stmt);
        i++;
    }

    WT_LOG_D << "Inserting " << (entries.size() - i) << " records in sqlite data base";
    for (; i < entries.size(); i++)
    {
        bool ret = sqlite3_bind_int64(insert_stmt, 1, entries[i].time_start) == SQLITE_OK;
        ret &= sqlite3_bind_int64(insert_stmt, 2, entries[i].time_end) == SQLITE_OK;
        ret &= sqlite3_bind_text(insert_stmt, 3, entries[i].proc_name.c_str(), -1, nullptr) == SQLITE_OK;
        ret &= sqlite3_bind_text(insert_stmt, 4, entries[i].description.c_str(), -1, nullptr) == SQLITE_OK;

        execute_statement(ret, insert_stmt);
    }

    last_entry = entries.back();
    entries.clear();
}

DataContainer SQLiteDataAccess::get_tree(DateRange period)
{
    container.clear();
    std::ostringstream sql_s;

    sql_s << "SELECT PROC_NAME, DESCRIPTION, SUM(TIME_END - TIME_START) FROM " << table_name << " ";

    if (period.from > 0 && period.to > 0)
    {
        sql_s << "WHERE TIME_START >= " << period.from << " AND TIME_END <=" << period.to << " ";
    }
    else if (period.from > 0)
    {
        sql_s << "WHERE TIME_START >= " << period.from << " ";
    }
    else if (period.to > 0)
    {
        sql_s << "WHERE TIME_END <= " << period.from << " ";
    }

    sql_s << "GROUP BY PROC_NAME, DESCRIPTION;";
    execute_query(sql_s.str(), query_container_callback);

    return container;
}

}
