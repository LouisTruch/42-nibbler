#pragma once
#include <vector>
#include <deque>

// Each Lib needs to be able to take at least these player's input
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
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