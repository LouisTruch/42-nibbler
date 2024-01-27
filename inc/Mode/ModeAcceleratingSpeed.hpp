#pragma once

#include "IMode.hpp"
#include <ctime> // std::clock_t

class ModeAcceleratingSpeed : public IMode
{
  public:
    ModeAcceleratingSpeed();
    ~ModeAcceleratingSpeed();
    ModeAcceleratingSpeed(const ModeAcceleratingSpeed &);
    ModeAcceleratingSpeed &operator=(const ModeAcceleratingSpeed &);

  public:
    bool check(const std::clock_t) noexcept override;
    void modifyGameSpeed(size_t &) noexcept;

  private:
    static inline constexpr long int _ACCELERATING_INTERVAL_MS = 5000;
    static inline constexpr std::size_t _GAME_SPEED_MODIFIER = 50;
    static inline constexpr std::size_t _GAME_SPEED_MIN = 200;
    std::clock_t _internalTimer;
};