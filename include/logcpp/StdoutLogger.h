#ifndef STDOUTLOGGER_CPP__
#define STDOUTLOGGER_CPP__

#include <logcpp/log.h>

#include <iostream>

namespace logcpp
{

class StdoutLogger : public Logger
{
public:
    StdoutLogger(bool withTimestamp = false)
        : Logger(), _withTimestamp(withTimestamp)
    {
    }
    virtual ~StdoutLogger() {}

private:
    virtual void logImpl(const std::string& timestamp, const std::string& label,
                         const std::string& message) override
    {
        if (_withTimestamp)
            std::cout << timestamp << " ";
        std::cout << label << ": " << message << std::endl;
    }

    bool _withTimestamp;
};
}

#endif /* STDOUTLOGGER_CPP */
