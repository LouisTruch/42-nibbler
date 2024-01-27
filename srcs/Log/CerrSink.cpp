#include "../../inc/Log/CerrSink.hpp"
#include <iostream>

void CerrSink::log(const std::string &message, const char *function, const char *file, int line)
{
    std::cerr << message << " " << function << " " << file << " " << line << "\e[0;37m" << std::endl;
}