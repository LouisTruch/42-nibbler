#pragma once

#include <deque>

typedef enum
{
    UP = -1,
    DOWN = 1,
    LEFT = -2,
    RIGHT = 2,
    SWAP_LIBNCURSES,
    SWAP_LIBSDL,
    SWAP_LIBRAYLIB,
    DEFAULT,
    QUIT,
} player_input_t;

struct point
{
    int x;
    int y;

    bool operator==(const point a) const
    {
        return (x == a.x && y == a.y);
    }
};
typedef struct point point_t;

typedef std::deque<point_t> body_t;

typedef int int_gameConfig_t;