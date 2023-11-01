#pragma once
#include <deque>
#include <vector>

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

enum map_objects
{
    OBJ_NONE,
    OBJ_PLAYER,
    OBJ_FOOD,
    OBJ_WALL,
};

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

enum item_name_idx_e
{
    SPEED,
    DISAPPEARING_FOOD,
    MULTIPLAYER,
    SOUND,
};

enum item_name_bit
{
    SPEED_BIT = 0x1,
    DISAPPEARING_FOOD_BIT = 0x2,
};