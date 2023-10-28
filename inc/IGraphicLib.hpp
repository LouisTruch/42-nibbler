#pragma once

#include "Player.hpp"
#include "types.hpp"
#include <memory>

class IGraphicLib
{
  public:
    virtual ~IGraphicLib() = default;
    virtual player_input_t getPlayerInput() const = 0;
    virtual void resetPlayerInput() = 0;
    virtual void registerPlayerInput() = 0;
    // Can't pass unique_ptr to .so functions ?
    virtual void drawPlayer(const Player &) = 0;
    virtual void drawFood(const point_t &) = 0;

  protected:
    player_input_t _playerInput = UP;
};