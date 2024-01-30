#pragma once

#include "../../../inc/Interface/IGraphicLib.hpp"

#include <deque>

class DebugGraphicLib : public IGraphicLib
{
  public:
    DebugGraphicLib(int, int);
    ~DebugGraphicLib();
    void clearBoard() const;
    player_input_t getPlayerInput(int) const;
    void resetPlayerInput();
    void registerPlayerInput() noexcept;
    void drawPlayer(const Snake_t &snake);
    void drawFood(const point_t &point);
    void drawScores(int, int);

  private:
    DebugGraphicLib() = delete;
    DebugGraphicLib(const DebugGraphicLib &) = delete;
    DebugGraphicLib &operator=(const DebugGraphicLib &) = delete;

  private:
    int _width;
    int _height;
    // Needed for this lib because we draw simply using std::cout 
    std::deque<point_t> _pBody;
    point_t _pFood;

  private:
    void drawPlayerFoodInSquare(std::deque<point_t> &pBody, point_t &pFood);
};

extern "C"
{
    DebugGraphicLib *makeGraphicLib(int width, int height);
    void destroyGraphicLib(DebugGraphicLib *gLib);
}