#pragma once

#include "../../../inc/Interface/IGraphicLib.hpp"
#include <ncurses.h>

#define PLAYER_0_PAIR 1
#define PLAYER_1_PAIR 2
#define FOOD_PAIR 2

constexpr char playerIcon = '*';
constexpr char playerHeadIcon = '@';
constexpr char foodIcon = 'o';
constexpr char boxIcon = '#';

class NcursesGraphicLib : public IGraphicLib
{
  public:
    NcursesGraphicLib(int, int);
    ~NcursesGraphicLib();
    NcursesGraphicLib(const NcursesGraphicLib &);
    NcursesGraphicLib &operator=(const NcursesGraphicLib &);
    void clearBoard() const;
    player_input_t getPlayerInput(int) const;
    void resetPlayerInput();
    void registerPlayerInput() noexcept;
    void drawPlayer(const Player &);
    void drawFood(const Food &);
    void drawScores(int, int);

  private:
    NcursesGraphicLib() = delete;
    WINDOW *_board;
    WINDOW *_scoreBoard;
    int _width;
    int _height;
};

extern "C"
{
    NcursesGraphicLib *makeGraphicLib(int width, int height);
    void destroyGraphicLib(NcursesGraphicLib *gLib);
}