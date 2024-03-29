#pragma once

#include "IMode.hpp"
#include <chrono> // std::chrono

class ModeMovingFood : public IMode
{
  public:
    ModeMovingFood();
    ~ModeMovingFood();
    ModeMovingFood(const ModeMovingFood &) = delete;
    ModeMovingFood &operator=(const ModeMovingFood &) = delete;

  public:
    std::uint16_t check(const std::chrono::time_point<std::chrono::high_resolution_clock> &) noexcept override;

  private:
    static inline constexpr long int _MOVING_INTERVAL_MS = 5000000;
};