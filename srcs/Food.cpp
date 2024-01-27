#include "../inc/Food.hpp"
#include "../inc/Log/Logger.hpp"

Food::Food(point_t spawn) : _pos(spawn)
{
    LOG_DEBUG("Food constructor at x:" << spawn.x << " y:" << spawn.y);
}

point_t Food::getPos() const
{
    return _pos;
}

Food::~Food()
{
    LOG_DEBUG("Food destructor");
}

Food::Food(const Food &other)
{
    *this = other;
}

Food &Food::operator=(const Food &other)
{
    if (&other == this)
        return *this;
    _pos = other._pos;
    return *this;
}