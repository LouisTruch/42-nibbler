#include "../inc/NcursesGraphicLib.hpp"
#include <curses.h>
#include <string>

NcursesGraphicLib::NcursesGraphicLib(int width, int height) : _width(width), _height(height)
{
    initscr();
    refresh();   // Clear Screen
    noecho();    // Prevent keypress being printed on screen
    curs_set(0); // Remove cursor from screen
    timeout(0);
    start_color();
    if (has_colors())
    {
        init_pair(PLAYER_0_PAIR, COLOR_RED, 0);
        init_pair(PLAYER_1_PAIR, COLOR_YELLOW, 0);
        init_pair(FOOD_PAIR, COLOR_GREEN, 0);
    }

    _board = newwin(_height, _width, 0, 0);
    _scoreBoard = newwin(3, 30, 1, _width + 1);
    keypad(_board, true);
    // nodelay(_board, TRUE);
    wtimeout(_board, 0); // Set non blocking call of user input
    box(_board, boxIcon, boxIcon);
    wrefresh(_board);
}

void NcursesGraphicLib::clearBoard() const
{
    wclear(_board);
    box(_board, boxIcon, boxIcon);
}

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
    }
}

void NcursesGraphicLib::drawPlayer(const Player &player)
{
    if (player._idx == 0)
        wattron(_board, COLOR_PAIR(PLAYER_0_PAIR));
    else if (player._idx == 1)
        wattron(_board, COLOR_PAIR(PLAYER_1_PAIR));
    for (auto &point : player._body._deque)
    {
        if (point == player._body._deque.front())
            mvwaddch(_board, point.y, point.x, playerHeadIcon);
        else
            mvwaddch(_board, point.y, point.x, playerIcon);
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

void NcursesGraphicLib::drawFood(const Food &food)
{
    wattron(_board, COLOR_PAIR(FOOD_PAIR));
    mvwaddch(_board, food._pos.y, food._pos.x, foodIcon);
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

NcursesGraphicLib::NcursesGraphicLib(const NcursesGraphicLib &other)
{
    *this = other;
}

NcursesGraphicLib &NcursesGraphicLib::operator=(const NcursesGraphicLib &other)
{
    if (&other == this)
        return *this;
    _width = other._width;
    _height = other._height;
    return *this;
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