#include "../../inc/Mode/ModeHandler.hpp"
#include "../../inc/Log/Logger.hpp"

#ifdef DEBUG
// This constructor is for debugging purpose only
ModeHandler::ModeHandler()
    : _modeState(
          {.hungerP0 = NOT_ACTIVE, .hungerP1 = NOT_ACTIVE, .movingFood = NOT_ACTIVE, .acceleratingSpeed = NOT_ACTIVE}),
      _hungerHandler(nullptr), _movingFoodHandler(nullptr), _acceleratingSpeedHandler(nullptr)
{
}
#endif

#include <iostream>
#include <memory> // std::make_unique
ModeHandler::ModeHandler(int_GameConfig_t config)
{
    LOG_DEBUG("CONFIG" + std::to_string(config));
    if (config & 0x1)
    {
        _acceleratingSpeedHandler = std::make_unique<ModeAcceleratingSpeed>();
        _modeState.acceleratingSpeed = ACTIVE;
    }
    else
    {
        _acceleratingSpeedHandler = nullptr;
        _modeState.acceleratingSpeed = NOT_ACTIVE;
    }
    if (config & 0x2)
    {
        _movingFoodHandler = std::make_unique<ModeMovingFood>();
        _modeState.movingFood = ACTIVE;
    }
    else
    {
        _movingFoodHandler = nullptr;
        _modeState.movingFood = NOT_ACTIVE;
    }
    if (config & 0x4)
    {
        _hungerHandler = std::make_unique<ModeHunger>();
        _modeState.hungerP0 = ACTIVE;
        _modeState.hungerP1 = ACTIVE;
    }
    else
    {
        _hungerHandler = nullptr;
        _modeState.hungerP0 = NOT_ACTIVE;
        _modeState.hungerP1 = NOT_ACTIVE;
    }
    LOG_DEBUG("Constructing ModeHandler " + this->getInfo());
}

ModeHandler::~ModeHandler()
{
    LOG_DEBUG("Destructing ModeHandler");
}

void ModeHandler::doCheckRoutine(const std::chrono::time_point<std::chrono::high_resolution_clock> &now)
{
    if (_modeState.hungerP0 == ACTIVE)
    {
        std::uint16_t hungerRet = _hungerHandler->check(now);
        if (hungerRet & 0x1)
        {
            _modeState.hungerP0 = CONDITION_MET;
        }
        if (hungerRet & 0x2)
        {
            _modeState.hungerP1 = CONDITION_MET;
        }
    }
    if (_modeState.movingFood == ACTIVE)
    {
        if (_movingFoodHandler->check(now))
        {
            _modeState.movingFood = CONDITION_MET;
        }
    }
    if (_modeState.acceleratingSpeed == ACTIVE)
    {
        if (_acceleratingSpeedHandler->check(now))
        {
            _modeState.acceleratingSpeed = CONDITION_MET;
        }
    }
}

void ModeHandler::resetHungerTimer(int playerIndex,
                                   const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept
{
    if (_hungerHandler != nullptr)
    {
        _hungerHandler->resetHungerTimer(playerIndex, now);
    }
}

void ModeHandler::modifyGameSpeed(long int &gameSpeed) noexcept
{
    if (_acceleratingSpeedHandler != nullptr)
    {
        _acceleratingSpeedHandler->modifyGameSpeed(gameSpeed);
    }
}

std::string ModeHandler::getInfo() const
{
    std::string info = "Info:";
    info += " Hunger:";
    info += (_hungerHandler != nullptr) ? "1" : "0";
    info += " MovingFood:";
    info += (_movingFoodHandler != nullptr) ? "1" : "0";
    info += " AcceleratingSpeed:";
    info += (_acceleratingSpeedHandler != nullptr) ? "1" : "0";
    return info;
}

ModeHandler::ModeCurrentState_e ModeHandler::getHungerState(int playerIndex) const noexcept
{
    if (playerIndex == 0)
    {
        return _modeState.hungerP0;
    }
    else if (playerIndex == 1)
    {
        return _modeState.hungerP1;
    }
    return NOT_ACTIVE;
}

ModeHandler::ModeCurrentState_e ModeHandler::getMovingFoodState() const noexcept
{
    return _modeState.movingFood;
}

ModeHandler::ModeCurrentState_e ModeHandler::getAcceleratingSpeedState() const noexcept
{
    return _modeState.acceleratingSpeed;
}

void ModeHandler::resetModeStates() noexcept
{
    if (_modeState.hungerP0 == CONDITION_MET)
    {
        _modeState.hungerP0 = ACTIVE;
    }
    if (_modeState.hungerP1 == CONDITION_MET)
    {
        _modeState.hungerP1 = ACTIVE;
    }
    if (_modeState.movingFood == CONDITION_MET)
    {
        _modeState.movingFood = ACTIVE;
    }
    if (_modeState.acceleratingSpeed == CONDITION_MET)
    {
        _modeState.acceleratingSpeed = ACTIVE;
    }
}