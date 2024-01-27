#pragma once

#include <string>

class ILogSink
{
  public:
    virtual ~ILogSink() = default;

    virtual void log(const std::string &message, const char *function, const char *file, int line) = 0;
};