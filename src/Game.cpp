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
        if (_gHandler->getPlayerInput() >= SWAP_LIBNCURSES && _gHandler->getPlayerInput() <= SWAP_LIBRAYLIB)
            handleLibSwitch();

        _turn = (clock() - _turnStart) / (double)CLOCKS_PER_SEC;
        if (_turn > DEFAULT_GAME_SPEED)
        {
            _player->move(_gHandler->getPlayerInput());
            if (checkCollision() == DEATH)
                break;
            _gHandler->drawPlayer(*_player);
            _gHandler->drawFood(_food->getPos());
            if (_totalSpace <= (int)_player->getBody().size())
                break;
            _turnStart = clock();
        }
    }
}

void Game::handleLibSwitch()
{
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

bool Game::isTileFree(point_t point)
{
    for (auto &playerPoint : _player->getBody())
        if (playerPoint == point)
            return false;
    return true;
}

point_t Game::chooseRandomFoodPos()
{
    point_t point = generateRandomPoint();
    while (!isTileFree((point = generateRandomPoint())))
        ;
    return point;
}

point_t Game::generateRandomPoint()
{
    std::uniform_int_distribution<int> rngWidth(0, _width - 1);
    std::uniform_int_distribution<int> rngHeight(0, _height - 1);
    return {rngWidth(_rng), (rngHeight(_rng))};
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
