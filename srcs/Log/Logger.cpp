#include "../../inc/Log/Logger.hpp"
#include "../../inc/Log/CerrSink.hpp"

Logger::Logger(LogLevel l, ILogSink &ls) : _logLevel(l), _logSink(ls)
{
}

void Logger::operator()(const std::string &message, const char *function, const char *file, int line)
{
    _logSink.log(message, function, file, line);
}

CerrSink CerrSink;
Logger &Debug()
{
    static Logger logger(LogLevel::LOG_LVL_DEFAULT, CerrSink);
    return logger;
}
