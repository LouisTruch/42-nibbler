#pragma once

#include "../../../inc/IGraphicLib.hpp"
#include <memory>
#include <ncurses.h>

constexpr char playerIcon = '@';
constexpr char foodIcon = 'o';
constexpr char boxIcon = '#';

class NcursesGraphicLib : public IGraphicLib
{
  public:
    NcursesGraphicLib(int, int);
    ~NcursesGraphicLib();
    NcursesGraphicLib(const NcursesGraphicLib &);
    NcursesGraphicLib &operator=(const NcursesGraphicLib &);
    player_input_t getPlayerInput() const;
    void resetPlayerInput();
    void registerPlayerInput();
    void drawPlayer(const Player &);
    void drawFood(const point_t &);

  private:
    NcursesGraphicLib();
    WINDOW *_board;
    int _width;
    int _height;
};

extern "C"
{
    std::unique_ptr<NcursesGraphicLib> makeGraphicLib(int width, int height);
    void destroyGraphicLib(std::unique_ptr<NcursesGraphicLib> gLib);
}