#ifndef LOGCPP_NOLOGGER_H__
#define LOGCPP_NOLOGGER_H__

namespace logcpp
{

class NoLogger : public Logger
{
public:
    virtual ~NoLogger() {}

private:
    virtual void logImpl(const std::string&, const std::string&,
                         const std::string&) override
    {
    }
};
}

#endif /* LOGCPP_NOLOGGER_H */
