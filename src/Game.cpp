#include "../inc/Game.hpp"
#include <ranges>
#include <unistd.h>

Game::Game(int width, int height) : _width(width), _height(height), _totalSpace(width * height)
{
    _rng.seed(getpid());
    _libHandler = std::make_unique<LibHandler>(_width, _height);
    _gHandler = _libHandler->makeGraphicLib(_width + 2, _height + 2);
    _player = std::make_unique<Player>(_width, _height, DEFAULT_PLAYER_SIZE);
    _food = std::make_unique<Food>(chooseRandomFoodPos());
}

void Game::loop(void)
{
    while (1)
    {
        _gHandler->registerPlayerInput();
        if (_gHandler->getPlayerInput() == QUIT)
            break;

        if (_gHandler->getPlayerInput() == SWAP_LIBNCURSES)
        {
            _gHandler = _libHandler->switchLib(LibHandler::LIBNCURSES, std::move(_gHandler));
            _gHandler->resetPlayerInput();
        }
        else if (_gHandler->getPlayerInput() == SWAP_LIBSDL)
        {
            _gHandler = _libHandler->switchLib(LibHandler::LIBSDL, std::move(_gHandler));
            _gHandler->resetPlayerInput();
        }

        _player->move(_gHandler->getPlayerInput());
        if (checkCollision() == DEATH)
            break;
        _gHandler->drawPlayer(_player->getBody());
        _gHandler->drawFood(_food->getPos());
        usleep(DEFAULT_GAME_SPEED);
    }
}

int Game::checkCollision()
{
    auto playerHeadPoint = _player->getHead();
    if (0 == playerHeadPoint->x || playerHeadPoint->x == _width + 3 || 0 == playerHeadPoint->y ||
        playerHeadPoint->y == _height + 3)
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
            point.x = i / _width;
            point.y = i % _height;
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
