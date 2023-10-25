#include "../inc/NcursesGraphicLib.hpp"

NcursesGraphicLib::NcursesGraphicLib(int width, int height) : _width(width + 2), _height(height + 2)
{
    initscr();
    refresh();   // Clear Screen
    noecho();    // Prevent keypress being printed on screen
    curs_set(0); // Remove cursor from screen
    _board = newwin(_height, _width, 0, 0);
    wtimeout(_board, 0); // Set non blocking call of user input
    timeout(0);
    box(_board, 0, 0);
    wrefresh(_board);
    keypad(_board, true);
}

void NcursesGraphicLib::getPlayerInput()
{
    chtype input = wgetch(_board);

    switch (input)
    {
    case 'w':
        playerInput = UP;
        break;
    case 's':
        playerInput = DOWN;
        break;
    case 'a':
        playerInput = LEFT;
        break;
    case 'd':
        playerInput = RIGHT;
        break;
    case '1':
        playerInput = SWAP_LIBNCURSES;
        break;
    case '2':
        playerInput = SWAP_LIBSDL;
        break;
    case 27:
        playerInput = QUIT;
        break;
    }
}

void NcursesGraphicLib::drawPlayer(const body_t &body)
{
    wclear(_board);
    box(_board, 0, 0);
    for (auto &point : body)
        mvwaddch(_board, point.y, point.x, playerIcon);
}

void NcursesGraphicLib::drawFood(const point_t &point)
{
    mvwaddch(_board, point.y, point.x, foodIcon);
}

NcursesGraphicLib::~NcursesGraphicLib()
{
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