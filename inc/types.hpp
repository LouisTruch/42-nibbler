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
    SWAP_LIB2,
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
    // point &operator=(const point &other)
    // {
    //     x = other.x;
    //     y = other.y;
    //     return *this;
    // }

    bool operator==(const point a) const
    {
        return (x == a.x && y == a.y);
    }
};
typedef struct point point_t;

typedef std::deque<point_t> body_t;