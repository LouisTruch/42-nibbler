#pragma once

#include "IMode.hpp"
#include <ctime> // std::clock_t

class ModeHunger : public IMode
{
  public:
    ModeHunger();
    ~ModeHunger();
    ModeHunger(const ModeHunger &);
    ModeHunger &operator=(const ModeHunger &);

  public:
    bool check(const std::clock_t) noexcept override;
    void resetHungerTimer(int playerIdex, const std::clock_t &now);

  private:
    static inline constexpr long int _MAX_HUNGER_TIMER_MS = 1000;
    std::clock_t _hungerTimerP0;
    std::clock_t _hungerTimerP1;
};