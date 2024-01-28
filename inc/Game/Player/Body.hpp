#pragma once

#include "../../types.hpp"
#include <deque>

class Body
{
  public:
    Body(point_t spawn, std::size_t size);
    ~Body();
    Body(const Body &);
    Body &operator=(const Body &);

    typedef enum
    {
        DIRECTION_UP = -1,
        DIRECTION_DOWN = 1,
        NO_DIRECTION = 0,
        DIRECTION_LEFT = -2,
        DIRECTION_RIGHT = 2
    } player_direction;

    void move(player_direction direction) noexcept;
    bool doesPointIntersectP(const point_t &point) const noexcept;
    void grow() noexcept;

  public:
    // Have to make them public because of symbol lookup error when using getters
    std::deque<point_t> _deque;

  private:
    Body() = delete;
};