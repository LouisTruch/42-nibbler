#pragma once

#include <deque>

typedef enum
{
    INPUT_UP = -1,
    INPUT_DOWN = 1,
    INPUT_DEFAULT = 0,
    INPUT_LEFT = -2,
    INPUT_RIGHT = 2,
    INPUT_SWAP_LIBNCURSES,
    INPUT_SWAP_LIBSDL,
    INPUT_SWAP_LIBRAYLIB,
    INPUT_QUIT
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
typedef struct point board_size_t; // Only for parsing

typedef int int_gameConfig_t;