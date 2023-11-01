#pragma once

#include "Food.hpp"
#include "types.hpp"
#include <ctime>
#include <memory>

class Game;

constexpr double SPEED_MODIFIER = 0.95;
constexpr double MAX_SPEED = 0.8;
constexpr double FOOD_TIMER = 4;

class ModesHandler
{
  public:
    ModesHandler(int_gameConfig_t);
    ~ModesHandler();
    ModesHandler(const ModesHandler &);
    ModesHandler &operator=(const ModesHandler &);
    void changeGameSpeed(double, Game &);
    std::unique_ptr<Food> handleDisappearingFood(Game &, std::unique_ptr<Food>);

  private:
    bool _isChangingSpeed;
    bool _isDisappearingFood;
    std::clock_t _foodTimer;
};