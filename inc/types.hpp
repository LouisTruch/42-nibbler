#pragma once

#ifndef DEBUG
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
    INPUT_SWAP_LIBSOUNDRAYLIB,
    INPUT_MUTE,
    INPUT_QUIT
} player_input_t;
#else
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
    INPUT_SWAP_LIBSOUNDRAYLIB,
    INPUT_SWAP_LIBSOUNDDEBUG,
    INPUT_MUTE,
    INPUT_QUIT,
    INPUT_SWAP_LIBDEBUG,
} player_input_t;
#endif

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
typedef struct point board_size_t;

typedef int int_GameConfig_t;

typedef enum
{
    DIRECTION_UP = -1,
    DIRECTION_DOWN = 1,
    NO_DIRECTION = 0,
    DIRECTION_LEFT = -2,
    DIRECTION_RIGHT = 2
} player_direction;

#include <deque>
typedef struct
{
    int idx;
    player_direction dir;
    std::deque<point_t> body;
} Snake_t;
#include <optional>
typedef struct
{
    Snake_t p0;
    std::optional<Snake_t> p1;
    point_t food;
    bool playEatingSound;
} GameData_t;