#include "../inc/Food.hpp"

#include <iostream>
Food::Food(point_t point)
{
    pos = point;
    std::cout << pos.x << " " << pos.y << std::endl;
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
    pos = other.pos;
    return *this;
}