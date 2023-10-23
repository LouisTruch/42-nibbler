#pragma once
#include "types.hpp"

class Food
{
public:
    Food(point_t);
    ~Food();
    Food(const Food &);
    Food &operator=(const Food &);
    point_t pos;

private:
    Food();
};