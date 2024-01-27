#pragma once

#include <sstream>
#include <string>

#define LOG(Logger_, Message_)                                                                                         \
    Logger_(                                                                                                           \
        static_cast<std::ostringstream &>(std::ostringstream().flush() << "\e[1;32m" << Message_ << "\e[0;32m").str(), \
        __FUNCTION__, __FILE__, __LINE__);

#ifdef NDEBUG
#define LOG_DEBUG(_)                                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
    } while (0)
#else
#define LOG_DEBUG(Message_) LOG(Debug(), Message_)
#endif

typedef enum
{
    LOG_LVL_DEFAULT,
} LogLevel;

class ILogSink;

class Logger
{
  public:
    Logger(LogLevel l, ILogSink &ls);
    ~Logger() = default;
    void operator()(const std::string &message, const char *function, const char *file, int line);

  private:
    LogLevel _logLevel;
    ILogSink &_logSink;
    Logger() = delete;
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};

Logger &Debug();