#pragma once
#include "types.hpp"

class Food
{
  public:
    Food(point_t);
    ~Food();
    Food(const Food &);
    Food &operator=(const Food &);
    point_t getPos() const;

  private:
    point_t _pos;

  private:
    Food();
};