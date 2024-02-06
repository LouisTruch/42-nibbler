#include "../inc/NcursesGraphicLib.hpp"
#include <curses.h>
#include <stdexcept> // std::runtime_error
#include <string>

#define PLAYER_0_PAIR 1
#define PLAYER_1_PAIR 2
#define FOOD_PAIR 2
NcursesGraphicLib::NcursesGraphicLib(int width, int height) : _width(width), _height(height)
{
    initscr();            // This function does not return when it fails
    if (refresh() == ERR) // Clear Screen
    {
        endwin();
        throw std::runtime_error("Error: initscr() failed");
    }
    if (noecho() == ERR) // Prevent keypress being printed on screen
    {
        endwin();
        throw std::runtime_error("Error: noecho() failed");
    }
    if (curs_set(0) == ERR) // Remove cursor from screen
    {
        endwin();
        throw std::runtime_error("Error: curs_set() failed");
    }
    timeout(0);
    if (start_color() == ERR)
    {
        endwin();
        throw std::runtime_error("Error: start_color() failed");
    }
    if (has_colors())
    {
        init_pair(PLAYER_0_PAIR, COLOR_RED, 0);
        init_pair(PLAYER_1_PAIR, COLOR_YELLOW, 0);
        init_pair(FOOD_PAIR, COLOR_GREEN, 0);
    }

    _board = newwin(_height, _width, 0, 0);
    if (_board == nullptr)
    {
        endwin();
        throw std::runtime_error("Error: newwin() failed");
    }
    _scoreBoard = newwin(3, 30, 1, _width + 1);
    if (_scoreBoard == nullptr)
    {
        delwin(_board);
        endwin();
        throw std::runtime_error("Error: newwin() failed");
    }
    if (keypad(_board, true) == ERR)
    {
        delwin(_board);
        delwin(_scoreBoard);
        endwin();
        throw std::runtime_error("Error: keypad() failed");
    }
    wtimeout(_board, 0);             // Set non blocking call of user input
    box(_board, _boxIcon, _boxIcon); // This only can fail if the win ptr is NULL
    if (wrefresh(_board) == ERR)
    {
        delwin(_board);
        delwin(_scoreBoard);
        endwin();
        throw std::runtime_error("Error: wrefresh() failed");
    }
}

void NcursesGraphicLib::clearBoard() const
{
    wclear(_board);
    box(_board, _boxIcon, _boxIcon);
}

#include <iostream>
void NcursesGraphicLib::registerPlayerInput() noexcept
{
    chtype input = wgetch(_board);

    switch (input)
    {
    case 'w':
        _arrayPlayerInput[0] = INPUT_UP;
        break;
    case 's':
        _arrayPlayerInput[0] = INPUT_DOWN;
        break;
    case 'a':
        _arrayPlayerInput[0] = INPUT_LEFT;
        break;
    case 'd':
        _arrayPlayerInput[0] = INPUT_RIGHT;
        break;
    case '2':
        _arrayPlayerInput[0] = INPUT_SWAP_LIBSDL;
        break;
    case '3':
        _arrayPlayerInput[0] = INPUT_SWAP_LIBRAYLIB;
        break;
    case KEY_UP:
        _arrayPlayerInput[1] = INPUT_UP;
        break;
    case KEY_DOWN:
        _arrayPlayerInput[1] = INPUT_DOWN;
        break;
    case KEY_LEFT:
        _arrayPlayerInput[1] = INPUT_LEFT;
        break;
    case KEY_RIGHT:
        _arrayPlayerInput[1] = INPUT_RIGHT;
        break;
    case 27:
        _arrayPlayerInput[0] = INPUT_QUIT;
        break;
#ifdef DEBUG
    case '1':
        _arrayPlayerInput[0] = INPUT_SWAP_LIBNCURSES;
        break;
    case '4':
        _arrayPlayerInput[0] = INPUT_SWAP_LIBDEBUG;
        break;
    case KEY_F(2):
        _arrayPlayerInput[0] = INPUT_SWAP_LIBSOUNDDEBUG;
        break;

#endif
    case KEY_F(1):
        _arrayPlayerInput[0] = INPUT_SWAP_LIBSOUNDRAYLIB;
        break;
    default:
        break;
    }
}

void NcursesGraphicLib::drawPlayer(const Snake_t &player)
{
    if (player.idx == 0)
        wattron(_board, COLOR_PAIR(PLAYER_0_PAIR));
    else if (player.idx == 1)
        wattron(_board, COLOR_PAIR(PLAYER_1_PAIR));
    for (auto &point : player.body)
    {
        if (point == player.body.front())
            mvwaddch(_board, point.y, point.x, _playerHeadIcon);
        else
            mvwaddch(_board, point.y, point.x, _playerIcon);
    }
    wattroff(_board, COLOR_PAIR(PLAYER_0_PAIR));
    wattroff(_board, COLOR_PAIR(PLAYER_1_PAIR));
}

void NcursesGraphicLib::drawScores(int score, int highScore)
{
    std::string str = std::to_string(score);
    mvwprintw(_scoreBoard, 0, 0, "%s", "SCORE: ");
    mvwprintw(_scoreBoard, 0, 7, "%s", str.c_str());
    str = std::to_string(highScore);
    mvwprintw(_scoreBoard, 2, 0, "%s", "HISCORE: ");
    wrefresh(_scoreBoard);
    mvwprintw(_scoreBoard, 2, 9, "%s", str.c_str());
    wrefresh(_scoreBoard);
}

void NcursesGraphicLib::drawFood(const point_t &point)
{
    wattron(_board, COLOR_PAIR(FOOD_PAIR));
    mvwaddch(_board, point.y, point.x, _foodIcon);
    wattroff(_board, COLOR_PAIR(FOOD_PAIR));
}

player_input_t NcursesGraphicLib::getPlayerInput(int playerIdx) const
{
    return _arrayPlayerInput[playerIdx];
}

void NcursesGraphicLib::resetPlayerInput()
{
    _arrayPlayerInput[0] = INPUT_DEFAULT;
    _arrayPlayerInput[1] = INPUT_DEFAULT;
}

NcursesGraphicLib::~NcursesGraphicLib()
{
    delwin(_board);
    delwin(_scoreBoard);
    endwin();
}

extern "C"
{
    NcursesGraphicLib *makeGraphicLib(int width, int height)
    {
        return new NcursesGraphicLib(width, height);
    }

    void destroyGraphicLib(NcursesGraphicLib *gLib)
    {
        delete gLib;
    }
}