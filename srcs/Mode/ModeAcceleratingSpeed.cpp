#include "../../inc/Mode/ModeAcceleratingSpeed.hpp"
#include "../../inc/Log/Logger.hpp"

ModeAcceleratingSpeed::ModeAcceleratingSpeed()
{
    LOG_DEBUG("Constructor");
    _internalClock = std::chrono::high_resolution_clock::now();
    _retValue = 0x4;
}

ModeAcceleratingSpeed::~ModeAcceleratingSpeed()
{
    LOG_DEBUG("Destructor");
}

std::uint16_t ModeAcceleratingSpeed::check(
    const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept
{
    if (getElapsedTimeInMs(now, _internalClock) >= _ACCELERATING_INTERVAL_MS)
    {
        LOG_DEBUG("Accelerating speed by" << _GAME_SPEED_MODIFIER);
        _internalClock = now;
        return _retValue;
    }
    return 0;
}

void ModeAcceleratingSpeed::modifyGameSpeed(long int &gameSpeed) noexcept
{
    if (gameSpeed > _MIN_GAME_SPEED_MS)
    {
        gameSpeed -= _GAME_SPEED_MODIFIER;
    }
}