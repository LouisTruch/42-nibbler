#pragma once
#include "Food.hpp"
#include "IGraphicLib.hpp"
#include "LibHandler.hpp"
#include "Player.hpp"
#include "types.hpp"
#include <bits/types/clock_t.h>
#include <memory>
#include <random>
#include <vector>

constexpr int MIN_WIDTH = 5;
constexpr int MAX_WIDTH = 47;
constexpr int MIN_HEIGHT = 5;
constexpr int MAX_HEIGHT = 24;
constexpr int DEFAULT_PLAYER_SIZE = 4;
constexpr double DEFAULT_GAME_SPEED = 0.15;

class Game
{
  public:
    Game(int, int);
    ~Game();
    Game(const Game &);
    Game &operator=(const Game &);

    void loop();
    int checkCollision();
    void handleLibSwitch();
    point_t chooseRandomFoodPos();
    point_t generateRandomPoint();
    bool isTileFree(point_t);

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
    double _turn;
    clock_t _turnStart;

    enum collision_type
    {
        DEATH,
        BUFF,
        NOTHING,
    };
};

void checkArgs(int, char **);
