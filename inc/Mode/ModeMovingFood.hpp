#pragma once

#include "IMode.hpp"
#include <ctime> // std::clock_t

class ModeMovingFood : public IMode
{
  public:
    ModeMovingFood();
    ~ModeMovingFood();
    ModeMovingFood(const ModeMovingFood &);
    ModeMovingFood &operator=(const ModeMovingFood &);

  public:
    bool check(const std::clock_t) noexcept override;

  private:
    static inline constexpr long int _MOVING_INTERVAL_MS = 5000;
    std::clock_t _internalTimer;
};