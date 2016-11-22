#include "sqlitedataaccess.h"
#include "logger.h"

#include <boost/filesystem.hpp>

#include <fstream>
#include <functional>
#include <stdexcept>

namespace WT {

SQLiteDataAccess::SQLiteDataAccess(const std::string &filename)
    : filename(filename)
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
    create_db = false;

    if (!boost::filesystem::exists(filename))
    {
        create_db = true;
    }

    if (!create_db)
    {
        int rc = sqlite3_open(filename.c_str(), &db);
        if (rc)
        {
            WT_LOG(LogLevel::DEBUG) << "unable to open database: " << sqlite3_errmsg(db);
            create_db = true;
        }
        else
        {
            create_db = true;
            execute_query("SELECT name FROM sqlite_master WHERE name='" + table_name + "'", &SQLiteDataAccess::db_created_callback);
            if (create_db)
            {
                sqlite3_close(db);
            }
        }
    }

    if (create_db)
    {
        if (readonly)
        {
            throw std::runtime_error("Cannot open database!");
        }
        else
        {
            create_database();
        }
    }
}

SQLiteDataAccess::~SQLiteDataAccess()
{
    sqlite3_close(db);
}

int SQLiteDataAccess::execute_query(const std::string &sql, sqlite3_callback callback)
{
    char *err_msg = nullptr;

    WT_LOG(LogLevel::DEBUG) << "Executing query " << sql;
    int rc = sqlite3_exec(db, sql.c_str(), callback, this, &err_msg);

    if (rc != SQLITE_OK)
    {
        WT_LOG(LogLevel::ERROR) << "Cannot execute query " << sql << ": " << err_msg;
        sqlite3_free(err_msg);
    }

    return rc;
}

void SQLiteDataAccess::create_database()
{
    WT_LOG(LogLevel::INFO) << "Database doesn't exist. Create database...";

    if (boost::filesystem::exists(filename))
    {
        auto rawtime = std::time(nullptr);
        auto timeinfo = std::localtime(&rawtime);
        char buf[32];
        strftime(buf, 32, ".%F_%H-%M-%S.bak", timeinfo);
        WT_LOG(LogLevel::DEBUG) << "Move file " << filename << " to " << filename + buf;
        boost::filesystem::rename(filename, filename + buf);
    }

    std::ofstream fs;
    fs.open(filename);
    fs.close();

    int rc = sqlite3_open(filename.c_str(), &db);
    if (rc)
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

    WT_LOG(LogLevel::INFO) << "Database succesfully created!";
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
}

void SQLiteDataAccess::persist_records()
{
    if (entries.empty())
    {
        return;
    }

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
