#pragma once
#include "Food.hpp"
#include "IGraphicLib.hpp"
#include "LibHandler.hpp"
#include "Player.hpp"
#include "types.hpp"
#include <memory>
#include <random>
#include <vector>

constexpr int DEFAULT_PLAYER_SIZE = 7;
constexpr int DEFAULT_GAME_SPEED = 100000;

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
    std::unique_ptr<LibHandler> _libHandler;
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