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

    // for (auto i : std::views::iota(0, (int)_map.size()))
    // {
    //     for (auto j : std::views::iota(0, (int)_map[i].size()))
    //     {
    //         std::cout << _map[i][j] + 0;
    //     }
    //     std::cout << '\n';
    // }
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