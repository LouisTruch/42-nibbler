#include "../inc/DebugGraphicLib.hpp"

#include <iostream>
DebugGraphicLib::DebugGraphicLib(int width, int height) : _width(width), _height(height)
{
    std::cout << "DebugGraphicLib::DebugGraphicLib(" << width << ", " << height << ")" << std::endl;
    _arrayPlayerInput[0] = INPUT_DEFAULT;
    _arrayPlayerInput[1] = INPUT_DEFAULT;
}

DebugGraphicLib::~DebugGraphicLib()
{
    std::cout << "DebugGraphicLib::~DebugGraphicLib()" << std::endl;
}

void DebugGraphicLib::clearBoard() const
{
    // std::cout << "DebugGraphicLib::clearBoard()" << std::endl;
}

player_input_t DebugGraphicLib::getPlayerInput(int player) const
{
    // std::cout << "DebugGraphicLib::getPlayerInput(" << player << ")" << std::endl;
    return _arrayPlayerInput[player];
}

void DebugGraphicLib::resetPlayerInput()
{
    // std::cout << "DebugGraphicLib::resetPlayerInput()" << std::endl;
    _arrayPlayerInput[0] = INPUT_DEFAULT;
    _arrayPlayerInput[1] = INPUT_DEFAULT;
}

void DebugGraphicLib::registerPlayerInput() noexcept
{
    std::cout << "DebugGraphicLib::registerPlayerInput()" << std::endl;
    char ret;
    std::cin >> ret;
    switch (ret)
    {
    case 'q':
        _arrayPlayerInput[0] = INPUT_QUIT;
        break;
    case 'w':
        _arrayPlayerInput[0] = INPUT_UP;
        break;
    case 'a':
        _arrayPlayerInput[0] = INPUT_LEFT;
        break;
    case 's':
        _arrayPlayerInput[0] = INPUT_DOWN;
        break;
    case 'd':
        _arrayPlayerInput[0] = INPUT_RIGHT;
        break;
    case '1':
        _arrayPlayerInput[0] = INPUT_SWAP_LIBNCURSES;
        break;
    case '2':
        _arrayPlayerInput[0] = INPUT_SWAP_LIBSDL;
        break;
    case '3':
        _arrayPlayerInput[0] = INPUT_SWAP_LIBRAYLIB;
        break;
    case '4':
        _arrayPlayerInput[0] = (player_input_t)8;
        break;
    }
    std::cout << "test: " << ret << " " << _arrayPlayerInput[0] << std::endl;
}

#include <vector>

void DebugGraphicLib::drawPlayer(const Player &player)
{
    _pBody = player._body._deque;
    std::cout << "DebugGraphicLib::drawPlayer()" << std::endl;
    std::cout << "Player " << player._idx << " :\n";
    // drawPlayerInSquare(player);
    // for (auto it = player._body._deque.begin(); it != player._body._deque.end(); ++it)
    // {
    //     std::cout << "  " << it->x << ", " << it->y << std::endl;
    // }
}

static void drawFrameAroundSquare(std::vector<std::vector<char>> &square)
{
    for (size_t i = 0; i < square.size(); ++i)
    {
        square[i][0] = '|';
        square[i][square[i].size() - 1] = '|';
    }
    for (size_t i = 0; i < square[0].size(); ++i)
    {
        square[0][i] = '-';
        square[square.size() - 1][i] = '-';
    }
    square[0][0] = '+';
    square[0][square[0].size() - 1] = '+';
    square[square.size() - 1][0] = '+';
    square[square.size() - 1][square[0].size() - 1] = '+';
}

void DebugGraphicLib::drawPlayerFoodInSquare(std::deque<point_t> &pBody, point_t &pFood)
{
    std::vector<std::vector<char>> square(_height, std::vector<char>(_width, ' '));
    for (auto it = pBody.begin(); it != pBody.end(); ++it)
    {
        square[it->y][it->x] = 'o';
    }
    square[pBody.front().y][pBody.front().x] = 'H';
    square[pBody.back().y][pBody.back().x] = 'T';
    square[pFood.y][pFood.x] = 'X';
    drawFrameAroundSquare(square);
    for (auto it = square.begin(); it != square.end(); ++it)
    {
        for (auto it2 = it->begin(); it2 != it->end(); ++it2)
        {
            std::cout << *it2;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void DebugGraphicLib::drawFood(const Food &point)
{
    (void)point;
    _pFood = point._pos;
    // std::cout << "DebugGraphicLib::drawFood()" << std::endl;
    std::cout << "Food : " << point._pos.x << ", " << point._pos.y << std::endl;
    drawPlayerFoodInSquare(_pBody, _pFood);
}

void DebugGraphicLib::drawScores(int score, int highScore)
{
    (void)score;
    (void)highScore;
    // std::cout << "DebugGraphicLib::drawScores()" << std::endl;
    // std::cout << "Score : " << score << std::endl;
    // std::cout << "HighScore : " << highScore << std::endl;
}

extern "C"
{
    DebugGraphicLib *makeGraphicLib(int width, int height)
    {
        return new DebugGraphicLib(width, height);
    }

    void destroyGraphicLib(DebugGraphicLib *gLib)
    {
        delete gLib;
    }
}
