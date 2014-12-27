log-cpp
=======

Lightweight logging for C++. Supports simple log file rotation.

Usage:

    #include <logcpp/log.h>
    #include <logcpp/FileLogger.h>

    using namespace std;
    using namespace logcpp;

    // In composition root/main
    auto maxLogfileSize = 1000000;
    Logger::setup(make_shared<FileLogger>("log.log", maxLogfileSize),
                  LogLevel::WARN);

    // In code
    Logger::debug("irrelevant ramblings"); // ignored as loglevel is WARN
    Logger::info("casual notes"); // ignored as loglevel is WARN
    Logger::warn("storm is coming");
    Logger::error("looks pretty bad");
