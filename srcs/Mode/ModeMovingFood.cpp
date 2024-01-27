#include "../../inc/Mode/ModeMovingFood.hpp"
#include "../../inc/Log/Logger.hpp"
#include <stdexcept> // std::runtime_error

ModeMovingFood::ModeMovingFood() : _internalTimer(std::clock())
{
    LOG_DEBUG("ModeMovingFood constructor");
    if (_internalTimer == (clock_t)(-1))
        throw std::runtime_error("ModeMovingFood(): std::clock() failed");
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
    _internalTimer = other._internalTimer;
    return *this;
}

bool ModeMovingFood::check(const std::clock_t now) noexcept
{
    if (now - _internalTimer >= _MOVING_INTERVAL_MS)
    {
        _internalTimer = now;
        return true;
    }
    return false;
}