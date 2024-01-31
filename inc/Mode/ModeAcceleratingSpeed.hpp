#pragma once

#include "IMode.hpp"
#include <chrono> // std::chrono
#include <ctime>  // std::clock_t

class ModeAcceleratingSpeed : public IMode
{
  public:
    ModeAcceleratingSpeed();
    ~ModeAcceleratingSpeed();
    ModeAcceleratingSpeed(const ModeAcceleratingSpeed &) = delete;
    ModeAcceleratingSpeed &operator=(const ModeAcceleratingSpeed &) = delete;

  public:
    std::uint16_t check(const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept override;
    void modifyGameSpeed(long int &) noexcept;

  private:
    static inline constexpr long int _ACCELERATING_INTERVAL_MS = 10000000;
    static inline constexpr long int _GAME_SPEED_MODIFIER = 50000;
    static inline constexpr long int _MIN_GAME_SPEED_MS = 100000;
};