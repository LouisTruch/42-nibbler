#pragma once

#include "../../../inc/Interface/IGraphicLib.hpp"
#include <ncurses.h>

class NcursesGraphicLib : public IGraphicLib
{
  public:
    NcursesGraphicLib(int, int);
    ~NcursesGraphicLib();

  public:
    void clearBoard() const;
    player_input_t getPlayerInput(int) const;
    void resetPlayerInput();
    void registerPlayerInput() noexcept;
    void drawPlayer(const Snake_t &snake);
    void drawFood(const point_t &point);
    void drawScores(int, int);

  private:
    int _width;
    int _height;
    WINDOW *_board;
    WINDOW *_scoreBoard;

  private:
    static inline constexpr char _playerIcon = '*';
    static inline constexpr char _playerHeadIcon = '@';
    static inline constexpr char _foodIcon = 'o';
    static inline constexpr char _boxIcon = '#';

  private:
    NcursesGraphicLib() = delete;
    NcursesGraphicLib(const NcursesGraphicLib &) = delete;
    NcursesGraphicLib &operator=(const NcursesGraphicLib &) = delete;
};

extern "C"
{
    NcursesGraphicLib *makeGraphicLib(int width, int height);
    void destroyGraphicLib(NcursesGraphicLib *gLib);
}