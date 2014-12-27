#ifndef LOGCPP_FILELOGGER_H__
#define LOGCPP_FILELOGGER_H__

#include <logcpp/log.h>

#include <fstream>
#include <sys/stat.h>

namespace logcpp
{

class FileLogger : public Logger
{
public:
    FileLogger(const std::string& logfilePath,
               const size_t fileSizeLimit = 500000)
        : Logger(),
          _logfilePath(logfilePath),
          _logfile(_logfilePath.c_str(), std::ios::app),
          _fileSizeLimit(fileSizeLimit)
    {
        check();
    }

    virtual ~FileLogger()
    {
        // _logfile.close() is implicit
    }

private:
    virtual void logImpl(const std::string& timestamp, const std::string& label,
                         const std::string& message) override
    {
        if (fileSize() > _fileSizeLimit)
            rotateLogfile();

        _logfile << timestamp << " " << label << ": " << message << std::endl;
        check();
        // TODO: _logfile.flush(); ?
    }

    void check() const
    {
        if (_logfile.fail())
            throw std::runtime_error("Failure in log file " + _logfilePath);
    }

    size_t fileSize() const
    {
        struct stat stat_buf;
        if (stat(_logfilePath.c_str(), &stat_buf) != 0)
            return 0;
        else
            return stat_buf.st_size;
    }

    void rotateLogfile()
    {
        _logfile.close();
        backupLogFile();
        _logfile = std::ofstream(_logfilePath.c_str(), std::ios::trunc);
        check();
    }

    void backupLogFile() const
    {
        std::ifstream src(_logfilePath, std::ios::binary);
        std::ofstream dst(_logfilePath + ".1", std::ios::binary);
        dst << src.rdbuf();
    }

    std::string _logfilePath;
    std::ofstream _logfile;
    size_t _fileSizeLimit;
};
}

#endif /* LOGCPP_FILELOGGER_H */
