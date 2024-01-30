#pragma once

#include <chrono> // std::chrono
// Using an interface because all mods follow the same simple pattern where you only
// have to check a value
class IMode
{
  public:
    virtual ~IMode() = default;
    virtual bool check(const std::chrono::time_point<std::chrono::high_resolution_clock> &) noexcept = 0;

  protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> _internalClock;
};

inline auto getElapsedTimeInMs(const std::chrono::time_point<std::chrono::high_resolution_clock> &clock1,
                               const std::chrono::time_point<std::chrono::high_resolution_clock> &clock2)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(clock1.time_since_epoch()).count() -
           std::chrono::duration_cast<std::chrono::microseconds>(clock2.time_since_epoch()).count();
}