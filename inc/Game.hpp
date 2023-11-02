#pragma once
#include "Food.hpp"
#include "IGraphicLib.hpp"
#include "ISoundLib.hpp"
#include "LibHandler.hpp"
#include "ModeHandler.hpp"
#include "Player.hpp"
#include "types.hpp"
#include <bits/types/clock_t.h>
#include <memory>
#include <random>
#include <string>
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
    typedef enum game_collision
    {
        NOTHING,
        RUNNING,
        DEATH_WALL,
        DEATH_BODY,
        EAT,
    } game_collision_e;

  public:
    Game(int, int, std::unique_ptr<ModeHandler>);
    ~Game();
    Game(const Game &);
    Game &operator=(const Game &);

    void loop();
    game_collision_e checkCollision();
    void handleLibSwitch();
    point_t chooseRandomFoodPos();
    point_t generateRandomPoint();
    bool isTileFree(point_t);
    void throwGameOverScore(std::string_view) const;
    double getGameSpeed() const;
    void setGameSpeed(double);

  private:
    Game() = delete;

  private:
    std::unique_ptr<LibHandler> _libHandler;
    std::unique_ptr<IGraphicLib> _graphicHandler;
    std::unique_ptr<Player> _player;
    std::unique_ptr<Food> _food;
    std::mt19937 _rng;
    int _width;
    int _height;
    int _totalSpace;
    double _gameSpeed;
    std::unique_ptr<ModeHandler> _modeHandler;
    double _turn;
    clock_t _turnStart;

  public:
    class GameOverException : public std::exception
    {
      private:
        std::string _msg;
        inline static constexpr std::string_view PLAYER_SCORE_STR = {"\nPlayer Score: "};
        inline static constexpr std::string_view MAP_WIDTH_STR = {"\nMap width: "};
        inline static constexpr std::string_view MAP_HEIGHT_STR = {"\nMap height: "};

      public:
        GameOverException(std::string_view, int, int, int);
        virtual const char *what() const throw();
        ~GameOverException() throw() = default;
    };
};

void checkArgs(int, char **);
