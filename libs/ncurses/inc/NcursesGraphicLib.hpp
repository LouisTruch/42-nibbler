#pragma once

#include "../../../inc/IGraphicLib.hpp"
#include <memory>
#include <ncurses.h>

constexpr char playerIcon = '#';
constexpr char foodIcon = 'o';

class NcursesGraphicLib : public IGraphicLib
{
  public:
    NcursesGraphicLib(int, int);
    ~NcursesGraphicLib();
    NcursesGraphicLib(const NcursesGraphicLib &);
    NcursesGraphicLib &operator=(const NcursesGraphicLib &);
    void drawPlayer(const body_t &);
    void drawFood(const point_t &);
    void getPlayerInput();

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