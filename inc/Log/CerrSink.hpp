#pragma once

#include "ILogSink.hpp"

class CerrSink : public ILogSink
{
  public:
    CerrSink() = default;
    ~CerrSink() = default;
    void log(const std::string &message, const char *function, const char *file, int line) override;
};