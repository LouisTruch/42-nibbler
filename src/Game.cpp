#include "../inc/Game.hpp"
#include <ranges>
#include <algorithm>

#include <iostream>

Game::Game(int width, int height)
{
    _map.resize(height);
    for (auto &&i : std::views::iota(0, height))
    {
        line_t line(width, 0);
        _map[i] = line;
    }

    _libHandler = std::make_unique<LibHandler>();
    _gHandler = _libHandler.get()->makeGraphicLib(width, height);
    _libHandler.get()->destroyGraphicLib(std::move(_gHandler));

    // _gHandler = _libHandler.get()->makeGraphicHandler(width, height);
    // for (auto i : std::views::iota(0, (int)_map.size()))
    // {
    //     for (auto j : std::views::iota(0, (int)_map[i].size()))
    //     {
    //         std::cout << _map[i][j] + 0;
    //     }
    //     std::cout << '\n';
    // }
}

void Game::loop(void)
{
    while (1)
    {
    }
}

Game::Game()
{
}

Game::~Game()
{
}

Game::Game(const Game &other)
{
    _map = other._map;
}

Game &Game::operator=(const Game &other)
{
    _map = other._map;
    return *this;
}