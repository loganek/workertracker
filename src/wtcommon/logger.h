#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <functional>
#include <type_traits>

namespace WT {

enum class LogLevel
{
    EMERGENCY = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4,
};

std::ostringstream& prepare_stream(std::ostringstream& os, LogLevel level);

template <typename TOutputPolicy>
class Logger : public TOutputPolicy
{
private:
    std::ostringstream os;
    LogLevel level;

    template<class>
    struct sfinae_true : std::true_type{};

    template<class T> static auto test_output(int) -> sfinae_true<decltype(std::declval<T>().output(std::declval<std::string>(), std::declval<WT::LogLevel>()))>;
    template<class> static auto test_output(long) -> std::false_type;

    void print(std::true_type) { TOutputPolicy::output(os.str()); }
    void print(std::false_type) { TOutputPolicy::output(os.str(), level); }

public:
    virtual ~Logger();

    Logger() = default;
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator =(const Logger&) = delete;
    Logger& operator =(Logger&&) = delete;

    std::ostringstream& get(LogLevel level = LogLevel::INFO);

    static LogLevel& reporting_level();
};


template <typename TOutputPolicy>
std::ostringstream& Logger<TOutputPolicy>::get(LogLevel level)
{
    this->level = level;
    return prepare_stream(os, level);
}

template <typename TOutputPolicy>
Logger<TOutputPolicy>::~Logger()
{
    os << std::endl;

    print(std::is_same<std::false_type, decltype(test_output<TOutputPolicy>(0))>{});
}

template <typename TOutputPolicy>
LogLevel& Logger<TOutputPolicy>::reporting_level()
{
    static LogLevel reportingLevel =
#if !defined(NDEBUG)
            LogLevel::DEBUG;
#else
            LogLevel::INFO;
#endif
    return reportingLevel;
}

class MethodOutput
{
private:
    using method_t = std::function<void(const std::string&, LogLevel)>;

    static method_t out_method;

public:
    static void output(const std::string& msg, LogLevel level);
    static void set_method(method_t method);
};

#define WT_IF_LOG(LOG_T, level) \
    if (level > LOG_T::reporting_level()); \
    else LOG_T().get(level)

class SysLogOutput
{
public:
    static void output(const std::string& msg, LogLevel level);
    static void init_log(const std::string &daemon_name);
};

#define WT_LOG(level) WT_IF_LOG(WT::Logger<WT::MethodOutput>, level)

#define WT_LOG_D WT_LOG(WT::LogLevel::DEBUG)
#define WT_LOG_EMG WT_LOG(WT::LogLevel::EMERGENCY)
#define WT_LOG_ERR WT_LOG(WT::LogLevel::ERROR)
#define WT_LOG_I WT_LOG(WT::LogLevel::INFO)
#define WT_LOG_W WT_LOG(WT::LogLevel::WARNING)
}

#endif // LOGGER_H
