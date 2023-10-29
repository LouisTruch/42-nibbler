#include "../inc/Game.hpp"
#include <ctime>
#include <iostream>
#include <ranges>
#include <unistd.h>

Game::Game(int width, int height) : _width(width), _height(height), _totalSpace(width * height)
{
    _rng.seed(getpid());
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    // +2 to includes border walls
    _gHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    _player = std::make_unique<Player>(width / 2, height / 2, DEFAULT_PLAYER_SIZE);
    _food = std::make_unique<Food>(chooseRandomFoodPos());
}

void Game::loop(void)
{
    _turnStart = clock();
    while (1)
    {
        _gHandler->registerPlayerInput();
        if (_gHandler->getPlayerInput() == QUIT)
            break;

        if (_gHandler->getPlayerInput() == SWAP_LIBNCURSES)
        {
            _gHandler = _libHandler->switchLib(LibHandler::LIBNCURSES, std::move(_gHandler));
            _gHandler->resetPlayerInput();
            _turnStart = clock();
        }
        else if (_gHandler->getPlayerInput() == SWAP_LIBSDL)
        {
            _gHandler = _libHandler->switchLib(LibHandler::LIBSDL, std::move(_gHandler));
            _gHandler->resetPlayerInput();
            _turnStart = clock();
        }
        else if (_gHandler->getPlayerInput() == SWAP_LIBRAYLIB)
        {
            _gHandler = _libHandler->switchLib(LibHandler::LIBRAYLIB, std::move(_gHandler));
            _gHandler->resetPlayerInput();
            _turnStart = clock();
        }

        _turn = (clock() - _turnStart) / (double)CLOCKS_PER_SEC;
        if (_turn > DEFAULT_GAME_SPEED)
        {
            _player->move(_gHandler->getPlayerInput());
            if (checkCollision() == DEATH)
                break;
            _gHandler->drawPlayer(*_player);
            _gHandler->drawFood(_food->getPos());
            _turnStart = clock();
            // Win
            if (_totalSpace == (int)_player->getBody().size())
                break;
        }
    }
}

int Game::checkCollision()
{
    auto playerHeadPoint = _player->getHead();
    if (0 > playerHeadPoint->x || playerHeadPoint->x >= _width || 0 > playerHeadPoint->y ||
        playerHeadPoint->y >= _height)
        return DEATH;

    if (*playerHeadPoint == _food->getPos())
    {
        _player->growBody();
        _food.reset(new Food(chooseRandomFoodPos()));
        return BUFF;
    }

    for (auto playerBodyIt = _player->getHead() + 1; playerBodyIt != _player->getTail(); ++playerBodyIt)
        if (*playerBodyIt == *playerHeadPoint)
            return DEATH;

    return NOTHING;
}

bool Game::isTileFree(int i)
{
    for (auto &point : _player->getBody())
    {
        int playerTileNb = point.x * _width + point.y;
        if (i == playerTileNb)
            return false;
    }
    return true;
}

point_t Game::chooseRandomFoodPos()
{
    std::uniform_int_distribution<int> int_dist(0, _totalSpace - _player->getBody().size());
    int freeTileNb = int_dist(_rng);
    point_t point = {-1, -1};
    for (auto i : std::views::iota(freeTileNb, _totalSpace))
    {
        if (isTileFree(i))
        {
            // ?? Need to try more
            point.x = i % _height;
            point.y = i / _width;
            return point;
        }
    }
    return point;
}

Game::Game()
{
    // ??
}

Game::~Game()
{
    _libHandler->destroyGraphicLib(std::move(_gHandler));
}

Game::Game(const Game &other)
{
    *this = other;
}

Game &Game::operator=(const Game &other)
{
    if (&other == this)
        return *this;
    _width = other._width;
    _height = other._height;
    _totalSpace = other._totalSpace;
    _rng = other._rng;
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    // Have to add copy _gHandler
    return *this;
}
