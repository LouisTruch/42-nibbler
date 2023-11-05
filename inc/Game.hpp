#pragma once
#include "Food.hpp"
#include "IGraphicLib.hpp"
#include "ISoundLib.hpp"
#include "LibHandler.hpp"
#include "ModeHandler.hpp"
#include "Player.hpp"
#include "types.hpp"
#include <array>
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
    typedef enum player_action
    {
        MOVE,
        DRAW,
        UPDATE_SCORE,
    } player_action_e;

  public:
    Game(std::unique_ptr<ModeHandler>);
    ~Game();
    Game(const Game &);
    Game &operator=(const Game &);

    void initPlayer();
    void loop();
    void playerAction(player_action_e);
    void checkCollisions() const;
    void checkPlayerState();
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
    std::unique_ptr<ModeHandler> _modeHandler;
    std::unique_ptr<LibHandler> _libHandler;
    std::unique_ptr<IGraphicLib> _graphicHandler;
    std::array<std::unique_ptr<Player>, 2> _arrayPlayer;
    std::unique_ptr<Food> _food;
    std::mt19937 _rng;
    int _width;
    int _height;
    int _totalSpace;
    double _gameSpeed;
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
