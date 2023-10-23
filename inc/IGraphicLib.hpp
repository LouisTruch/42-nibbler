#pragma once

#include <iostream>
#include <vector>
#include "types.hpp"

class IGraphicLib
{
public:
    player_input_t playerInput = UP;

public:
    virtual ~IGraphicLib() = default;
    virtual void drawMap(const map_t &) = 0;
    virtual void getPlayerInput() = 0;
};