#include "../inc/Game.hpp"
#include <ranges>
#include <algorithm>

#include <iostream>

Game::Game(int width, int height) : _width(width), _height(height)
{
    _map.resize(_height);
    for (auto &&i : std::views::iota(0, _height))
    {
        line_t line(_width, OBJ_NONE);
        _map[i] = line;
    }

    _gHandler = _libHandler.makeGraphicLib(_width, _height);
    _player = std::make_unique<Player>(_width, _height, DEFAULT_PLAYER_SIZE);
}

void Game::loop(void)
{
    while (1)
    {
        _gHandler->getPlayerInput();
        if (_gHandler->playerInput == QUIT)
            break;
        _player->move(_gHandler->playerInput);
        resetMap();
        putPlayerOnMap();
        _gHandler->drawMap(_map);
        usleep(DEFAULT_GAME_SPEED);
    }
}

void Game::resetMap()
{
    for (int i = 0; i < (int)_map.size(); i++)
    {
        for (int j = 0; j < (int)_map[i].size(); j++)
        {
            _map[i][j] = 0;
        }
    }
}

void Game::putPlayerOnMap()
{
    for (auto &&point : _player.get()->body)
        _map[point.x][point.y] = OBJ_PLAYER;
}

Game::Game()
{
    // ??
}

Game::~Game()
{
    _libHandler.destroyGraphicLib(std::move(_gHandler));
}

Game::Game(const Game &other)
{
    *this = other;
}

Game &Game::operator=(const Game &other)
{
    if (&other == this)
        return *this;
    _map = other._map;
    _width = other._width;
    _height = other._height;
    // Have to add copy of _libH and _gHandler
    return *this;
}