#ifndef LOGCPP_LOG_H__
#define LOGCPP_LOG_H__

#include <ctime>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

namespace logcpp
{

enum class LogLevel {
    DEBUG = 1,
    INFO,
    WARN,
    ERROR,
};

inline std::string toString(const LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            break;
    }
    throw std::logic_error("Unmapped log level");
}

class Logger
{
public:
    virtual ~Logger() {}

    static void setup(std::shared_ptr<Logger> logger, LogLevel level);

    // TODO: need macro for __FUNCTION__ etc...
    static void debug(const std::string& message)
    {
        instance().log(message, LogLevel::DEBUG);
    }

    static void info(const std::string& message)
    {
        instance().log(message, LogLevel::INFO);
    }

    static void warn(const std::string& message)
    {
        instance().log(message, LogLevel::WARN);
    }

    static void error(const std::string& message)
    {
        instance().log(message, LogLevel::ERROR);
    }

protected:
    Logger() {}

    virtual void logImpl(const std::string& timestamp, const std::string& label,
                         const std::string& message) = 0;

    static Logger& instance();

    void log(const std::string& message, const LogLevel level)
    {
        static std::mutex m;

        if (level >= _logLevel) {
            std::lock_guard<std::mutex> hold(m);
            logImpl(getTimestamp(), toString(level), message);
        }
    }

private:
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

    LogLevel _logLevel;

    static std::string getTimestamp()
    {
        // %z is unfortunately not reliable in Windows...
        auto response = std::string("0000-00-00 00:00:00");
        std::vector<char> buf(response.size() + 1);
        auto t = std::time(nullptr);
#ifdef _WIN32
        auto timeinfo = std::tm{};
        localtime_s(&timeinfo, &t);
#else
        auto timeinfo = std::localtime(&t);
#endif
        if (std::strftime(&buf[0], buf.size(), "%Y-%m-%d %H:%M:%S", &timeinfo))
            response = std::string(begin(buf), end(buf) - 1); // omit '\0'
        return response;
    }
};
}

#endif /* LOGCPP_LOG_H */
