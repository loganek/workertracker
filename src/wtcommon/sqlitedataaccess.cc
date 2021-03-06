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
#include "configuration.h"

#include <boost/filesystem.hpp>
#include <boost/io/detail/quoted_manip.hpp>

extern "C" {

static void sqlite3_ext_regexp(sqlite3_context *ctx, int, sqlite3_value **argv)
{
    auto regexp = (const char *) sqlite3_value_text(argv[0]);
    auto value = (const char *) sqlite3_value_text(argv[1]);

    try
    {
        auto res = std::regex_search(value, std::regex(regexp));
        sqlite3_result_int(ctx, res);
    }
    catch(const std::exception& e)
    {
        sqlite3_result_error(ctx, e.what(), -1);
    }
}

}

namespace WT {

SQLiteDataAccess::SQLiteDataAccess(const std::string &data_file)
     : filename(data_file)
{
}

std::string SQLiteDataAccess::default_data_file()
{
    return (boost::filesystem::path(WT::Configuration::get_default_config_path()).parent_path() / "data.dat").string();
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

void SQLiteDataAccess::init_sqlite3()
{
    prepare_statements();
    create_regexp_function();
}

void SQLiteDataAccess::create_regexp_function()
{
    sqlite3_create_function_v2(db, "regexp", 2, SQLITE_UTF8, nullptr,
                               sqlite3_ext_regexp, nullptr, nullptr, nullptr);
}

void SQLiteDataAccess::prepare_statements()
{
    auto insert_sql = "INSERT INTO " + table_name + "(TIME_START, TIME_END, PROC_NAME, DESCRIPTION) VALUES(?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &insert_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Can't prepare insert statement");
    }

    auto update_sql = "UPDATE " + table_name + " set TIME_END = ? WHERE TIME_END = ?;";
    if (sqlite3_prepare_v2(db, update_sql.c_str(), -1, &update_stmt, nullptr) != SQLITE_OK)
    {
        throw std::runtime_error("Can't prepare update statement");
    }
}

void SQLiteDataAccess::start_persist_worker(bool readonly)
{
    if (readonly)
    {
        WT_LOG_D << "Database opened in readonly mode, persist worker won't start.";
        return;
    }

    is_running = true;
    persist_worker_th = std::thread([this] {
        WT_LOG_D << "Starting persist worker thread...";
        this->persist_worker();
        WT_LOG_D << "Finishing persist worker thread...";
    });
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
            init_sqlite3();
            start_persist_worker(readonly);
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
    init_sqlite3();
    start_persist_worker(readonly);
}

SQLiteDataAccess::~SQLiteDataAccess()
{
    if (db)
    {
        if (is_running)
        {
            is_running = false;
            cv.notify_one();
            persist_worker_th.join();
        }
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

void SQLiteDataAccess::save_entry(const DataEntry &entry)
{
    {
        std::lock_guard<std::mutex> lck(mtx);
        data_queue.push(entry);
    }
    cv.notify_one();
}

void SQLiteDataAccess::persist_worker()
{
    while (true)
    {
        DataEntry entry;
        {
            std::unique_lock<std::mutex> lck(mtx);
            while (data_queue.empty() && is_running)
            {
                cv.wait(lck);
            }

            if (!is_running)
            {
                WT_LOG_D << "Finishing persist worker...";
                return;
            }
            entry = data_queue.front();
            data_queue.pop();
        }
        persist_record(entry);
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

void SQLiteDataAccess::persist_record(const DataEntry& entry)
{
    if (last_entry.is_continuous(entry)
                && std::localtime(&last_entry.time_start)->tm_hour == std::localtime(&entry.time_start)->tm_hour)
    {
        WT_LOG_D << "Updating entry";
        bool ret = sqlite3_bind_int64(update_stmt, 1, entry.time_end) == SQLITE_OK;
        ret &= sqlite3_bind_int64(update_stmt, 2, entry.time_start) == SQLITE_OK;

        execute_statement(ret, update_stmt);
    }
    else
    {
        WT_LOG_D << "Inserting record to a sqlite data base";

        bool ret = sqlite3_bind_int64(insert_stmt, 1, entry.time_start) == SQLITE_OK;
        ret &= sqlite3_bind_int64(insert_stmt, 2, entry.time_end) == SQLITE_OK;
        ret &= sqlite3_bind_text(insert_stmt, 3, entry.proc_name.c_str(), -1, nullptr) == SQLITE_OK;
        ret &= sqlite3_bind_text(insert_stmt, 4, entry.description.c_str(), -1, nullptr) == SQLITE_OK;

        execute_statement(ret, insert_stmt);
    }

    last_entry = entry;
}

std::string SQLiteDataAccess::translate_variable_name(const std::string &var_name)
{
    static std::unordered_map<std::string, std::string> var_map = {
        {"name", "proc_name"},
        {"description", "description"},
        {"time", "time_start"},
        {"weekday", "cast(strftime('%w', datetime(time_start, 'unixepoch', 'localtime')) as integer)"},
        {"hour", "cast(strftime('%H', datetime(time_start, 'unixepoch', 'localtime')) as integer)"},
        {"start", "time_start"},
        {"start_weekday", "cast(strftime('%w', datetime(time_start, 'unixepoch', 'localtime')) as integer)"},
        {"start_hour", "cast(strftime('%H', datetime(time_start, 'unixepoch', 'localtime')) as integer)"},
        {"end", "time_end"},
        {"end_weekday", "cast(strftime('%w', datetime(time_start, 'unixepoch', 'localtime')) as integer)"},
        {"end_hour", "cast(strftime('%H', datetime(time_start, 'unixepoch', 'localtime')) as integer)"}
    };

    return var_map[var_name];
}

std::string SQLiteDataAccess::translate_operator(Operator op)
{
    switch (op)
    {
    case Operator::OR: return "OR";
    case Operator::AND: return "AND";
    case Operator::EQ : return "=";
    case Operator::NEQ: return "!=";
    case Operator::MATCH: return "regexp";
    case Operator::GE: return ">=";
    case Operator::GT: return ">";
    case Operator::LE: return "<=";
    case Operator::LT: return "<";
    default: throw std::runtime_error("unknown operator");
    }
}

struct SQLiteValueVisitor : boost::static_visitor<void>
{
    std::ostream &stream;
    SQLiteValueVisitor(std::ostream& stream) : stream(stream) {}

    void operator()(const std::tm & val) const { stream << std::mktime(const_cast<std::tm*>(&val)); }
    void operator()(const std::string &val) const { stream << boost::io::quoted(val, '\'', '\''); }
    template <typename T> void operator()(const T& val) const { stream << val; }
};


void SQLiteDataAccess::load_expression_condition(std::shared_ptr<Operand> op, std::ostream& stream)
{
    if (auto bin_expr = std::dynamic_pointer_cast<BinaryExpression>(op))
    {
        stream << "(";
        load_expression_condition(bin_expr->get_left_operand(), stream);
        stream << " " << translate_operator(bin_expr->get_operator()) << " ";
        load_expression_condition(bin_expr->get_right_operand(), stream);
        stream << ")";
    }
    else if (auto variable = std::dynamic_pointer_cast<VariableOperand>(op))
    {
        stream << translate_variable_name(variable->get_name());
    }
    else
    {
        auto v = op->get_value();
        boost::apply_visitor(SQLiteValueVisitor(stream), v);
    }
}

DataContainer SQLiteDataAccess::get_entries(const std::shared_ptr<BinaryExpression>& expression)
{
    DataContainer out;

    std::ostringstream sql_s;

    sql_s << "SELECT PROC_NAME, DESCRIPTION, TIME_START, TIME_END FROM " << table_name << " ";

    if (expression)
    {
        sql_s << "WHERE ";
        load_expression_condition(expression, sql_s);
    }

    sql_s << " ORDER BY TIME_START;";
    std::string sql_str = sql_s.str();

    sqlite3_stmt *select_stmt;
    sqlite3_prepare_v2(db, sql_str.c_str(), -1, &select_stmt, nullptr);
    int status;
    do
    {
        switch (status = sqlite3_step(select_stmt))
        {
        case SQLITE_ROW:
        {
            out.emplace_back(
                        reinterpret_cast<const char*>(sqlite3_column_text(select_stmt, 0)),
                        reinterpret_cast<const char*>(sqlite3_column_text(select_stmt, 1)),
                        sqlite3_column_int64(select_stmt, 2),
                        sqlite3_column_int64(select_stmt, 3));
        }
        case SQLITE_DONE:
            break;
        default:
            WT_LOG_ERR << "Error on reading data";
            break;
        }
    } while (status == SQLITE_ROW);
    sqlite3_finalize(select_stmt);

    return out;
}

}
