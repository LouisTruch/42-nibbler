#include "../../inc/Mode/ModeMovingFood.hpp"
#include "../../inc/Log/Logger.hpp"
#include <stdexcept> // std::runtime_error

ModeMovingFood::ModeMovingFood()
{
    LOG_DEBUG("ModeMovingFood constructor");
    _internalClock = std::chrono::high_resolution_clock::now();
}

ModeMovingFood::~ModeMovingFood()
{
    LOG_DEBUG("ModeMovingFood destructor");
}

ModeMovingFood::ModeMovingFood(const ModeMovingFood &other)
{
    *this = other;
}

ModeMovingFood &ModeMovingFood::operator=(const ModeMovingFood &other)
{
    if (&other == this)
        return *this;
    _internalClock = other._internalClock;
    return *this;
}

bool ModeMovingFood::check(const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept
{
    if (getElapsedTimeInMs(now, _internalClock) >= _MOVING_INTERVAL_MS)
    {
        LOG_DEBUG("Moving food");
        _internalClock = now;
        return true;
    }
    return false;
}