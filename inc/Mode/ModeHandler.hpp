#pragma once

#include "../types.hpp"
#include "ModeAcceleratingSpeed.hpp"
#include "ModeHunger.hpp"
#include "ModeMovingFood.hpp"
#include <chrono> // std::chrono
#include <ctime>  // std::clock_t
#include <memory> // std::unique_ptr
#include <string> // std::string

class ModeHandler
{
  public:
    ModeHandler();
    ModeHandler(int_gameConfig_t);
    ~ModeHandler();
    ModeHandler(const ModeHandler &) = delete;
    ModeHandler &operator=(const ModeHandler &) = delete;

  public:
    size_t checkRoutine(const std::chrono::time_point<std::chrono::high_resolution_clock> &now);
    void resetHungerTimer(int playerIdx,
                          const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept;
    void modifyGameSpeed(long int &) noexcept;
    void updateScore(size_t &);

  private:
    std::string getInfo() const;

  private:
    std::unique_ptr<ModeHunger> _hungerHandler;
    std::unique_ptr<ModeMovingFood> _movingFoodHandler;
    std::unique_ptr<ModeAcceleratingSpeed> _acceleratingSpeedHandler;
};