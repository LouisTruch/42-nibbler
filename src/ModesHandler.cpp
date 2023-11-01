#include "../inc/ModesHandler.hpp"
#include "../inc/Game.hpp"
#include <ctime>
#include <iostream>
#include <memory>

ModesHandler::ModesHandler(int_gameConfig_t config)
{
    _isChangingSpeed = config & SPEED_BIT ? true : false;
    _isDisappearingFood = config & DISAPPEARING_FOOD_BIT ? true : false;
    if (_isDisappearingFood)
        _foodTimer = std::clock();
}

ModesHandler::~ModesHandler()
{
}

ModesHandler::ModesHandler(const ModesHandler &other)
{
    *this = other;
}

ModesHandler &ModesHandler::operator=(const ModesHandler &other)
{
    if (&other == this)
        return *this;
    _isChangingSpeed = other._isChangingSpeed;
    _isDisappearingFood = other._isDisappearingFood;
    _foodTimer = other._foodTimer;
    return *this;
}

void ModesHandler::changeGameSpeed(double coeff, Game &game)
{
    if (!_isChangingSpeed)
        return;
    if (game.getGameSpeed() >= MAX_SPEED)
        return;
    game.setGameSpeed(game.getGameSpeed() * coeff);
}

std::unique_ptr<Food> ModesHandler::handleDisappearingFood(Game &game, std::unique_ptr<Food> food)
{
    // std::cout << (_foodTimer - std::clock()) / (double)CLOCKS_PER_SEC << "test\n";
    if (!_isDisappearingFood || !(((std::clock() - _foodTimer) / (double)CLOCKS_PER_SEC) >= FOOD_TIMER))
        return food;
    _foodTimer = std::clock();
    return std::make_unique<Food>(game.chooseRandomFoodPos());
}