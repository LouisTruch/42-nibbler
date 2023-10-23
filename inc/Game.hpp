#pragma once
#include <memory>
#include <vector>
#include "types.hpp"
#include "LibHandler.hpp"
#include "IGraphicLib.hpp"
#include "Player.hpp"

constexpr int DEFAULT_PLAYER_SIZE = 3;
constexpr int DEFAULT_GAME_SPEED = 50000;

class Game
{
public:
    Game(int, int);
    ~Game();
    Game(const Game &);
    Game &operator=(const Game &);

    void loop();
    void resetMap();
    void putPlayerOnMap();

private:
    Game();

private:
    LibHandler _libHandler;
    std::unique_ptr<IGraphicLib> _gHandler;
    std::unique_ptr<Player> _player;
    map_t _map;
    int _width;
    int _height;
};