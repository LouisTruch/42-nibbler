#pragma once

#include "../types.hpp"

class Food
{
  public:
    Food(point_t spawn);
    ~Food();
    Food(const Food &);
    Food &operator=(const Food &);
    point_t getPos() const;

  public:
    // Have to make them public because of symbol lookup error when using getters
    point_t _pos;

  private:
    Food() = delete;
};