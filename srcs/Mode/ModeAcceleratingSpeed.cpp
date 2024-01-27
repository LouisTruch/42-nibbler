#include "../../inc/Mode/ModeAcceleratingSpeed.hpp"
#include "../../inc/Log/Logger.hpp"
#include <stdexcept> // std::runtime_error

ModeAcceleratingSpeed::ModeAcceleratingSpeed() : _internalTimer(std::clock())
{
    LOG_DEBUG("ModeAcceleratingSpeed constructor");
    if (_internalTimer == (clock_t)(-1))
        throw std::runtime_error("ModeAcceleratingSpeed(): std::clock() failed");
}

ModeAcceleratingSpeed::~ModeAcceleratingSpeed()
{
    LOG_DEBUG("ModeAcceleratingSpeed destructor");
}

ModeAcceleratingSpeed::ModeAcceleratingSpeed(const ModeAcceleratingSpeed &other)
{
    *this = other;
}

ModeAcceleratingSpeed &ModeAcceleratingSpeed::operator=(const ModeAcceleratingSpeed &other)
{
    if (&other == this)
        return *this;
    _internalTimer = other._internalTimer;
    return *this;
}

bool ModeAcceleratingSpeed::check(const std::clock_t now) noexcept
{
    if (now - _internalTimer >= _ACCELERATING_INTERVAL_MS)
    {
        _internalTimer = now;
        return true;
    }
    return false;
}

void ModeAcceleratingSpeed::modifyGameSpeed(size_t &gameSpeed) noexcept
{
    if (gameSpeed > _GAME_SPEED_MIN)
    {
        gameSpeed -= _GAME_SPEED_MODIFIER;
    }
}