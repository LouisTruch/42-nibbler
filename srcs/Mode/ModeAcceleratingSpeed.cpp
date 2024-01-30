#include "../../inc/Mode/ModeAcceleratingSpeed.hpp"
#include "../../inc/Log/Logger.hpp"

ModeAcceleratingSpeed::ModeAcceleratingSpeed()
{
    LOG_DEBUG("Constructor");
    _internalClock = std::chrono::high_resolution_clock::now();
}

ModeAcceleratingSpeed::~ModeAcceleratingSpeed()
{
    LOG_DEBUG("Destructor");
}

ModeAcceleratingSpeed::ModeAcceleratingSpeed(const ModeAcceleratingSpeed &other)
{
    *this = other;
}

ModeAcceleratingSpeed &ModeAcceleratingSpeed::operator=(const ModeAcceleratingSpeed &other)
{
    if (&other == this)
        return *this;
    _internalClock = other._internalClock;
    return *this;
}

bool ModeAcceleratingSpeed::check(const std::chrono::time_point<std::chrono::high_resolution_clock> &now) noexcept
{
    if (getElapsedTimeInMs(now, _internalClock) >= _ACCELERATING_INTERVAL_MS)
    {
        LOG_DEBUG("Accelerating speed by" << _GAME_SPEED_MODIFIER);
        _internalClock = now;
        return true;
    }
    return false;
}

void ModeAcceleratingSpeed::modifyGameSpeed(long int &gameSpeed) noexcept
{
    if (gameSpeed > _MIN_GAME_SPEED_MS)
    {
        gameSpeed -= _GAME_SPEED_MODIFIER;
    }
}