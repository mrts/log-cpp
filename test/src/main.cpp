/**
 * Copyright (c) 2014 Mart Sõmermaa <mart.somermaa@eesti.ee>
 *
 * MIT license.
 */

#include <logcpp/log.h>
#include <logcpp/FileLogger.h>
#include <logcpp/GetAppDataPath.h>

#if defined(_WIN32) && defined(ERROR)
// including ShlObj.h causes global namespace pollution, clean it up
#undef ERROR
#endif

#include <testcpp/testcpp.h>

using namespace std;
using namespace logcpp;

class MockLogger : public Logger
{
public:
    virtual ~MockLogger() {}
    std::string message;

private:
    virtual void logImpl(const std::string&, const std::string& label,
                         const std::string& msg) override
    {
        message = label + ":" + msg;
    }
};

class TestLogging : public Test::Suite
{
public:
    TESTCPP_TYPEDEFS(TestLogging)

    virtual void test()
    {
        testLogLevels();
        testFileLogging();
    }

private:
    void testLogLevels()
    {
        auto mockLogger = make_shared<MockLogger>();

        Logger::setup(mockLogger, LogLevel::DEBUG);
        Logger::debug("debug-log");
        assertEqual(mockLogger->message, "DEBUG:debug-log");
        Logger::info("info-log");
        assertEqual(mockLogger->message, "INFO:info-log");
        Logger::warn("warn-log");
        assertEqual(mockLogger->message, "WARN:warn-log");
        Logger::error("error-log");
        assertEqual(mockLogger->message, "ERROR:error-log");

        Logger::setup(mockLogger, LogLevel::INFO);
        mockLogger->message = "empty";
        Logger::debug("debug-log");
        assertEqual(mockLogger->message, "empty");
        Logger::info("info-log");
        assertEqual(mockLogger->message, "INFO:info-log");

        Logger::setup(mockLogger, LogLevel::WARN);
        mockLogger->message = "empty";
        Logger::debug("debug-log");
        assertEqual(mockLogger->message, "empty");
        Logger::info("info-log");
        assertEqual(mockLogger->message, "empty");
        Logger::warn("warn-log");
        assertEqual(mockLogger->message, "WARN:warn-log");

        Logger::setup(mockLogger, LogLevel::ERROR);
        mockLogger->message = "empty";
        Logger::debug("debug-log");
        assertEqual(mockLogger->message, "empty");
        Logger::info("info-log");
        assertEqual(mockLogger->message, "empty");
        Logger::warn("warn-log");
        assertEqual(mockLogger->message, "empty");
        Logger::error("error-log");
        assertEqual(mockLogger->message, "ERROR:error-log");
    }

    void testFileLogging()
    {
        auto tmpfile = GetAppDataPath("log-cpp", "filelogger.tmp.log");
        remove(tmpfile.c_str());
        auto mockLogger = make_shared<MockLogger>();

        Logger::setup(make_shared<FileLogger>(tmpfile), LogLevel::DEBUG);
        Logger::info("nothing special");
        // free and close the file logger
        Logger::setup(mockLogger, LogLevel::DEBUG);

        auto tmpfileSize = fileSize(tmpfile);
#ifdef _WIN32
        // len('2014-12-27 22:13:05 INFO: nothing special') -> 41
        auto expectedTmpfileSize = 43u; // CRLF
#else
        auto expectedTmpfileSize = 42u; // LF
#endif
        assertEqual(tmpfileSize, expectedTmpfileSize);

        // test rotation
        Logger::setup(make_shared<FileLogger>(tmpfile, 40u), LogLevel::DEBUG);
        Logger::info("this will cause rotation before write");
        Logger::setup(mockLogger, LogLevel::DEBUG);

        tmpfileSize = fileSize(tmpfile);
        auto rotatedFile = tmpfile + ".1";
        auto rotatedFileSize = fileSize(rotatedFile);
#ifdef _WIN32
        auto expectedNewFileSize = 65u; // CRLF
#else
        auto expectedNewFileSize = 64u; // LF
#endif
        assertEqual(rotatedFileSize, expectedTmpfileSize);
        assertEqual(tmpfileSize, expectedNewFileSize);

        remove(tmpfile.c_str());
        remove(rotatedFile.c_str());
    }

    size_t fileSize(const std::string& filename) const
    {
        struct stat stat_buf;
        if (stat(filename.c_str(), &stat_buf) != 0)
            throw runtime_error("stat() failed for " + filename);
        return stat_buf.st_size;
    }
};

int main()
{
    Test::Controller& controller = Test::Controller::instance();
    controller.addTestSuite("Logger tests", Test::Suite::instance<TestLogging>);
    return controller.run();
}
