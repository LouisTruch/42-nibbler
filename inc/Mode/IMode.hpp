#pragma once

#include <ctime> // std::clock_t
// Using an interface because all mods follow the same simple pattern where you only
// have to check a value
class IMode
{
  public:
    virtual ~IMode() = default;

    virtual bool check(const std::clock_t) noexcept = 0;
};