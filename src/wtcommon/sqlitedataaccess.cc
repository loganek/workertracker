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

SQLiteDataAccess::SQLiteDataAccess(const std::string &filename)
    // TODO store_cnt from configuration!
    : store_cnt(5),
      filename(filename)
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

int SQLiteDataAccess::db_created_callback(void *data_access, int argc, char **argv, char ** /*col_name*/)
{
    SQLiteDataAccess* this_ = reinterpret_cast<SQLiteDataAccess*>(data_access);

    this_->create_db = (argc == 0 || argv[0] != this_->table_name);

   return 0;
}

void SQLiteDataAccess::open(bool readonly)
{
    this->readonly = readonly; // TODO use this flag for opening database

    if (boost::filesystem::exists(filename))
    {
        if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
        {
            WT_LOG_W << "unable to open database: " << sqlite3_errmsg(db);
        }
        else
        {
            create_db = true;
            execute_query("SELECT name FROM sqlite_master WHERE name='" + table_name + "'", &SQLiteDataAccess::db_created_callback);
            if (!create_db)
            {
                return;
            }
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
}

SQLiteDataAccess::~SQLiteDataAccess()
{
    if (db)
    {
        persist_records();
        sqlite3_close(db);
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

    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
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

void SQLiteDataAccess::persist_records()
{
    if (entries.empty())
    {
        WT_LOG_D << "Nothing to persist";
        return;
    }

    WT_LOG_D << "Persisiting records";

    // TODO transactions
    std::ostringstream sql_s;
    std::size_t i = 0;

    if (is_continuous_entry(last_entry, entries.front()))
    {
        sql_s << "UPDATE " << table_name<< " set TIME_END = "
              << entries.back().time_end << " WHERE TIME_END = " << entries.back().time_start << "; ";
        i++;
        WT_LOG(LogLevel::DEBUG) << "Updating entry";
    }

    for (; i < entries.size(); i++)
    {
        sql_s << "INSERT INTO " << table_name << "(TIME_START, TIME_END, PROC_NAME, DESCRIPTION) VALUES("
              << entries[i].time_start << ", " << entries[i].time_end << ", '" << entries[i].proc_name << "', '" << entries[i].description << "');";
    }

    WT_LOG(LogLevel::DEBUG) << "Persisting " << entries.size() << " records in sqlite data base";
    execute_query(sql_s.str().c_str());

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
