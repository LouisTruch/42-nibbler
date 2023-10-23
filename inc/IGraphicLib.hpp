#pragma once

#include <iostream>
#include <vector>

class IGraphicLib
{
public:
    // Each Graphic Lib needs a map to draw on its window from
    typedef u_int8_t tile_t;
    typedef std::vector<tile_t> line_t;
    typedef std::vector<std::vector<tile_t>> map_t;

    // Each Lib needs to be able to take at least these player's input
    typedef enum
    {
        DEFAULT,
        UP,
        DOWN,
        RIGHT,
        LEFT,
        QUIT,
    } player_input_t;
    player_input_t playerInput;

public:
    virtual ~IGraphicLib() = default;
    virtual void drawMap(const map_t &) = 0;
    virtual void getPlayerInput() = 0;
};