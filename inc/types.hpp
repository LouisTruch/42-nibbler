#pragma once
#include <vector>

// Each Graphic Lib needs a map to draw on its window from
typedef u_int8_t tile_t;
typedef std::vector<tile_t> line_t;
typedef std::vector<std::vector<tile_t>> map_t;

// Each Lib needs to be able to take at least these player's input
typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    QUIT,
    DEFAULT,
} player_input_t;

enum map_objects
{
    OBJ_NONE,
    OBJ_PLAYER,
    OBJ_FOOD,
    OBJ_WALL,
};