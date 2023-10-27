#include "../inc/NcursesGraphicLib.hpp"
// #include <sys/ioctl.h>
// #include <unistd.h>

NcursesGraphicLib::NcursesGraphicLib(int width, int height) : _width(width + 2), _height(height + 2)
{
    initscr();
    refresh();   // Clear Screen
    noecho();    // Prevent keypress being printed on screen
    curs_set(0); // Remove cursor from screen
    timeout(0);

    // struct winsize w;
    // if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
    _board = newwin(_height, _width, 0, 0);
    // else
    // _board = newwin(_height, _width, w.ws_row / 2 - (_height / 2), w.ws_col / 2 - (_width / 2));

    wtimeout(_board, 0); // Set non blocking call of user input
    box(_board, 0, 0);
    wrefresh(_board);
    keypad(_board, true);
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
    case '1':
        _playerInput = SWAP_LIBNCURSES;
        break;
    case '2':
        _playerInput = SWAP_LIBSDL;
        break;
    case '3':
        _playerInput = SWAP_LIBSFML;
        break;
    case 27:
        _playerInput = QUIT;
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