#pragma once

#include <chrono> // std::chrono
class IMode
{
  public:
    virtual ~IMode() = default;
    virtual std::uint16_t check(const std::chrono::time_point<std::chrono::high_resolution_clock> &) noexcept = 0;

  protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> _internalClock;
    std::uint16_t _retValue;
};

inline auto getElapsedTimeInMs(const std::chrono::time_point<std::chrono::high_resolution_clock> &clock1,
                               const std::chrono::time_point<std::chrono::high_resolution_clock> &clock2)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(clock1.time_since_epoch()).count() -
           std::chrono::duration_cast<std::chrono::microseconds>(clock2.time_since_epoch()).count();
}