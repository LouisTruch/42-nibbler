#pragma once

#include "IMode.hpp"
#include <chrono> // std::chrono

class ModeHunger : public IMode
{
  public:
    ModeHunger();
    ~ModeHunger();
    ModeHunger(const ModeHunger &) = delete;
    ModeHunger &operator=(const ModeHunger &) = delete;

  public:
    std::uint16_t check(const std::chrono::time_point<std::chrono::high_resolution_clock> &) noexcept override;
    void resetHungerTimer(int playerIdex, const std::chrono::time_point<std::chrono::high_resolution_clock> &);

  private:
    static inline constexpr long int _MAX_HUNGER_TIMER_MS = 4000000;
    std::chrono::time_point<std::chrono::high_resolution_clock> _internalClock2;
};