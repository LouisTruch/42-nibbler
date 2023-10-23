#pragma once
#include <memory>
#include <vector>
#include <random>
#include "types.hpp"
#include "LibHandler.hpp"
#include "IGraphicLib.hpp"
#include "Player.hpp"
#include "Food.hpp"

constexpr int DEFAULT_PLAYER_SIZE = 5;
constexpr int DEFAULT_GAME_SPEED = 50000;

class Game
{
public:
    Game(int, int);
    ~Game();
    Game(const Game &);
    Game &operator=(const Game &);

    void loop();
    int checkCollision();
    point_t chooseRandomFoodPos();
    bool isTileFree(int);

private:
    Game();

private:
    LibHandler _libHandler;
    std::unique_ptr<IGraphicLib> _gHandler;
    std::unique_ptr<Player> _player;
    std::unique_ptr<Food> _food;
    std::mt19937 _rng;
    int _width;
    int _height;
    int _totalSpace;

    enum collision_type
    {
        DEATH,
        BUFF,
        NOTHING,
    };
};