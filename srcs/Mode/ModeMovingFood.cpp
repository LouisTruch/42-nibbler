#include "../../inc/Mode/ModeMovingFood.hpp"
#include "../../inc/Log/Logger.hpp"
#include <stdexcept> // std::runtime_error

ModeMovingFood::ModeMovingFood() : IMode()
{
    LOG_DEBUG("ModeMovingFood constructor");
    _internalClock = std::chrono::high_resolution_clock::now();
    _retValue = 0x8;
}

ModeMovingFood::~ModeMovingFood()
{
    LOG_DEBUG("ModeMovingFood destructor");
}

std::uint16_t ModeMovingFood::check(const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept
{
    if (getElapsedTimeInMs(now, _internalClock) >= _MOVING_INTERVAL_MS)
    {
        LOG_DEBUG("Moving food");
        _internalClock = now;
        return _retValue;
    }
    return 0;
}