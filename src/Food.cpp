#include "../inc/Food.hpp"

Food::Food(point_t point) : _pos(point)
{
}

point_t Food::getPos() const
{
    return _pos;
}

Food::Food()
{
}

Food::~Food()
{
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