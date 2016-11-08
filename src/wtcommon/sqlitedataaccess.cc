#include "sqlitedataaccess.h"
#include "logger.h"

#include <boost/filesystem.hpp>

#include <functional>
#include <stdexcept>

namespace WT {

SQLiteDataAccess::SQLiteDataAccess(const std::string &filename)
    : filename(filename)
{
}

int SQLiteDataAccess::db_created_callback(void *data_access, int argc, char **argv, char ** /*col_name*/)
{
    SQLiteDataAccess* this_ = reinterpret_cast<SQLiteDataAccess*>(data_access);

    this_->create_db = (argc == 0 || argv[0] != this_->table_name);

   return 0;
}

void SQLiteDataAccess::open()
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
        create_database();
    }
}

SQLiteDataAccess::~SQLiteDataAccess()
{
    sqlite3_close(db);
}

int SQLiteDataAccess::execute_query(const std::string &sql, sqlite3_callback callback)
{
    char *err_msg = nullptr;
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
                      "TIME         INTEGER PRIMARY KEY  NOT NULL," \
                      "PROC_NAME    CHAR(50)," \
                      "DESCRIPTION  CHAR(150));";

    if (execute_query(sql) != SQLITE_OK)
    {
        throw std::runtime_error("Cannot create database!");
    }

    WT_LOG(LogLevel::INFO) << "Database succesfully created!";
}

void SQLiteDataAccess::save_entry(const DataEntry &entry)
{
    entries.push_back(entry);
}

void SQLiteDataAccess::persist_records()
{
    std::ostringstream sql_s;

    for (const auto& entry : entries)
    {
        sql_s << "INSERT INTO WT_ENTRIES(TIME, PROC_NAME, DESCRIPTION) VALUES(" << entry.time << ", '" << entry.proc_name << "', '" << entry.description << "');";
    }

    execute_query(sql_s.str().c_str());

    entries.clear();

    execute_query("select * from WT_ENTRIES;");
}

}
