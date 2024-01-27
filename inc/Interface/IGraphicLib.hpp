#pragma once

#include "../Food.hpp"
#include "../Player/Player.hpp"
#include "../types.hpp"
#include <array>
#include <memory>

class IGraphicLib
{
  public:
    typedef std::array<player_input_t, 2> ArrayPlayerInput;

    virtual ~IGraphicLib() = default;
    virtual void clearBoard() const = 0;
    virtual player_input_t getPlayerInput(int) const = 0;
    virtual void resetPlayerInput() = 0;
    virtual void registerPlayerInput() noexcept = 0;
    // Can't pass unique_ptr to .so loaded functions ?
    virtual void drawPlayer(const Player &) = 0;
    virtual void drawFood(const Food &) = 0;
    virtual void drawScores(int, int) = 0;

  protected:
    ArrayPlayerInput _arrayPlayerInput = {INPUT_UP, INPUT_UP};
};