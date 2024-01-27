#include "../../inc/Mode/ModeHunger.hpp"
#include "../../inc/Log/Logger.hpp"
#include <stdexcept> // std::runtime_error

ModeHunger::ModeHunger() : _hungerTimerP0(std::clock()), _hungerTimerP1(std::clock())
{
    LOG_DEBUG("ModeHunger constructor " + std::to_string(_MAX_HUNGER_TIMER_MS) + "ms");
    if (_hungerTimerP0 == (clock_t)(-1))
        throw std::runtime_error("ModeHunger(): std::clock() failed");
    if (_hungerTimerP1 == (clock_t)(-1))
        throw std::runtime_error("ModeHunger(): std::clock() failed");
}

ModeHunger::~ModeHunger()
{
}

ModeHunger::ModeHunger(const ModeHunger &other)
{
    *this = other;
}

ModeHunger &ModeHunger::operator=(const ModeHunger &other)
{
    if (&other == this)
        return *this;
    _hungerTimerP0 = other._hungerTimerP0;
    return *this;
}

#include <iostream>
bool ModeHunger::check(const std::clock_t time) noexcept
{
    std::cout << "ModeHunger::check(" << time << ")" << std::endl;
    if (time - _hungerTimerP0 >= _MAX_HUNGER_TIMER_MS)
    {
        return true;
    }
    if (time - _hungerTimerP1 >= _MAX_HUNGER_TIMER_MS)
    {
        return true | 0x2;
    }
    return false;
}

void ModeHunger::resetHungerTimer(int playerIdex, const std::clock_t &now)
{
    std::cout << "ModeHunger::resetHungerTimer(" << now << ")" << std::endl;
    if (playerIdex == 0)
        _hungerTimerP0 = now;
    else if (playerIdex == 1)
        _hungerTimerP1 = now;
    else
        throw std::runtime_error("ModeHunger::resetHungerTimer(): playerIdex is not 0 or 1");
}