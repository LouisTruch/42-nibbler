#include "../../inc/Mode/ModeHunger.hpp"
#include "../../inc/Log/Logger.hpp"

ModeHunger::ModeHunger()
{
    LOG_DEBUG("Constructor");
    _internalClock = std::chrono::high_resolution_clock::now();
    _internalClock2 = std::chrono::high_resolution_clock::now();
    _retValue = 0x1;
}

ModeHunger::~ModeHunger()
{
}

std::uint16_t ModeHunger::check(const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept
{
    std::uint16_t ret = 0;
    if (getElapsedTimeInMs(now, _internalClock) > _MAX_HUNGER_TIMER_MS)
    {
        LOG_DEBUG("Hunger timer 0 expired");
        ret = _retValue;
    }
    if (getElapsedTimeInMs(now, _internalClock2) > _MAX_HUNGER_TIMER_MS)
    {
        LOG_DEBUG("Hunger timer 1 expired");
        ret |= _retValue << 1;
    }
    return ret;
}

void ModeHunger::resetHungerTimer(int playerIdx, const std::chrono::time_point<std::chrono::high_resolution_clock> &now)
{
    if (playerIdx == 0)
    {
        LOG_DEBUG("Resetting hunger timer 0");
        _internalClock = now;
    }
    else if (playerIdx == 1)
    {
        LOG_DEBUG("Resetting hunger timer 1");
        _internalClock2 = now;
    }
    else
        throw std::runtime_error("ModeHunger::resetHungerTimer(): playerIdex is not 0 or 1");
}