#include <logcpp/log.h>
#include <logcpp/NoLogger.h>

#include <ioccpp/ioc.h>
#include <ioccpp/IoCContainerImpl.hh>

namespace logcpp
{

template class IoCContainer<Logger>;

void Logger::setup(std::shared_ptr<Logger> logger, LogLevel level)
{
    logger->_logLevel = level;
    IoCContainer<Logger>::Register(logger);
}

Logger& Logger::instance()
{
    if (!IoCContainer<Logger>::DoesInstanceExist())
        IoCContainer<Logger>::Register(std::make_shared<NoLogger>());
    return IoCContainer<Logger>::Resolve();
}
}
