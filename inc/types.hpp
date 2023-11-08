#pragma once

#include <deque>

typedef enum
{
    UP = -1,
    DOWN = 1,
    DEFAULT = 0,
    LEFT = -2,
    RIGHT = 2,
    SWAP_LIBNCURSES,
    SWAP_LIBSDL,
    SWAP_LIBRAYLIB,
    QUIT,
} player_input_t;

typedef enum
{
    STATE_NOTHING,
    STATE_WALL_COLL,
    STATE_BODY_COLL,
    STATE_FOOD,
    STATE_PLAYER_COLL,
    STATE_HUNGER
} player_state_e;

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

typedef std::deque<point_t> body_t;

typedef int int_gameConfig_t;