#include "../inc/NcursesGraphicLib.hpp"
#include <curses.h>

NcursesGraphicLib::NcursesGraphicLib(int width, int height) : _width(width), _height(height)
{
    initscr();
    refresh();   // Clear Screen
    noecho();    // Prevent keypress being printed on screen
    curs_set(0); // Remove cursor from screen
    timeout(0);

    _board = newwin(_height, _width, 0, 0);
    // keypad(_board, true);
    // nodelay(_board, TRUE);
    wtimeout(_board, 0); // Set non blocking call of user input
    box(_board, boxIcon, boxIcon);
    wrefresh(_board);
}

void NcursesGraphicLib::registerPlayerInput()
{
    chtype input = wgetch(_board);

    switch (input)
    {
    case 'w':
        _playerInput = UP;
        break;
    case 's':
        _playerInput = DOWN;
        break;
    case 'a':
        _playerInput = LEFT;
        break;
    case 'd':
        _playerInput = RIGHT;
        break;
    case '2':
        _playerInput = SWAP_LIBSDL;
        break;
    case '3':
        _playerInput = SWAP_LIBRAYLIB;
        break;
    case 27:
        _playerInput = QUIT;
        break;
    }
}

void NcursesGraphicLib::drawPlayer(const Player &player)
{
    wclear(_board);
    box(_board, boxIcon, boxIcon);
    for (auto &point : player._body)
    {
        if (point == player._body.front())
            mvwaddch(_board, point.y + 1, point.x + 1, playerHeadIcon);
        else
            mvwaddch(_board, point.y + 1, point.x + 1, playerIcon);
    }
}

void NcursesGraphicLib::drawFood(const point_t &point)
{
    mvwaddch(_board, point.y + 1, point.x + 1, foodIcon);
}

player_input_t NcursesGraphicLib::getPlayerInput() const
{
    return _playerInput;
}

void NcursesGraphicLib::resetPlayerInput()
{
    _playerInput = DEFAULT;
}

NcursesGraphicLib::~NcursesGraphicLib()
{
    delwin(_board);
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

NcursesGraphicLib::NcursesGraphicLib()
{
}

std::unique_ptr<NcursesGraphicLib> makeGraphicLib(int width, int height)
{
    return std::make_unique<NcursesGraphicLib>(width, height);
}

void destroyGraphicLib(std::unique_ptr<NcursesGraphicLib> gLib)
{
    gLib.reset();
}