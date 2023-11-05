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

void NcursesGraphicLib::registerPlayerInput()
{
    chtype input = wgetch(_board);

    switch (input)
    {
    case 'w':
        _arrayPlayerInput[0] = UP;
        break;
    case 's':
        _arrayPlayerInput[0] = DOWN;
        break;
    case 'a':
        _arrayPlayerInput[0] = LEFT;
        break;
    case 'd':
        _arrayPlayerInput[0] = RIGHT;
        break;
    case '2':
        _arrayPlayerInput[0] = SWAP_LIBSDL;
        break;
    case '3':
        _arrayPlayerInput[0] = SWAP_LIBRAYLIB;
        break;
    case KEY_UP:
        _arrayPlayerInput[1] = UP;
        break;
    case KEY_DOWN:
        _arrayPlayerInput[1] = DOWN;
        break;
    case KEY_LEFT:
        _arrayPlayerInput[1] = LEFT;
        break;
    case KEY_RIGHT:
        _arrayPlayerInput[1] = RIGHT;
        break;
    case 27:
        _arrayPlayerInput[0] = QUIT;
        break;
    }
}

void NcursesGraphicLib::drawPlayer(const Player &player)
{
    // if (has_colors())
    // {
    if (player._playerIdx == 0)
        wattron(_board, COLOR_PAIR(PLAYER_0_PAIR));
    else
        wattron(_board, COLOR_PAIR(PLAYER_1_PAIR));
    for (auto &point : player._body)
    {
        if (point == player._body.front())
            mvwaddch(_board, point.y + 1, point.x + 1, playerHeadIcon);
        else
            mvwaddch(_board, point.y + 1, point.x + 1, playerIcon);
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
    mvwaddch(_board, point.y + 1, point.x + 1, foodIcon);
    wattroff(_board, COLOR_PAIR(FOOD_PAIR));
}

player_input_t NcursesGraphicLib::getPlayerInput(int playerIdx) const
{
    return _arrayPlayerInput[playerIdx];
}

void NcursesGraphicLib::resetPlayerInput()
{
    _arrayPlayerInput[0] = DEFAULT;
    _arrayPlayerInput[1] = DEFAULT;
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

std::unique_ptr<NcursesGraphicLib> makeGraphicLib(int width, int height)
{
    return std::make_unique<NcursesGraphicLib>(width, height);
}

void destroyGraphicLib(std::unique_ptr<NcursesGraphicLib> gLib)
{
    gLib.reset();
}