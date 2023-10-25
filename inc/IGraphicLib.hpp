#pragma once

#include "Player.hpp"
#include "types.hpp"
#include <memory>

class IGraphicLib
{
  public:
    player_input_t playerInput = UP;

  public:
    virtual ~IGraphicLib() = default;
    virtual void drawPlayer(const body_t &) = 0;
    virtual void drawFood(const point_t &) = 0;
    virtual void getPlayerInput() = 0;
};