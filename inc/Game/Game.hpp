#pragma once

#include "../Mode/ModeHandler.hpp"
#include "../types.hpp"
#include "./Player/Player.hpp"
#include "Food.hpp"
#include "Score.hpp"
#include <chrono>      // std::chrono::system_clock
#include <random>      // std::mt19937
#include <string_view> // std::string_view

class Game
{
  public:
    enum collision_type
    {
        NO_COLLISION,
        COLLISION_FOOD,
        COLLISION_ITSELF,
        COLLISION_WALL,
        COLLISION_OTHERPLAYER
    };

  public:
    Game(board_size_t boardSize, std::unique_ptr<ModeHandler>, bool multiplayer);
    ~Game();
    Game() = delete;
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

  public:
    void playTurn();
    void setShouldPlayEatingSound(bool) noexcept;
    bool getShouldPlayEatingSound() const noexcept;
    std::shared_ptr<Player> getP(std::size_t idx);
    std::shared_ptr<Player> getP0();
    std::shared_ptr<Player> getP1();
    std::shared_ptr<Food> getFood();
    const GameData_t exportData() const;
    bool isScore() const noexcept;
    int getCurrentScore() const noexcept;
    int getHighScore() const noexcept;

  public:
    class GameOverException : public std::exception
    {
      public:
        std::string msg;
        GameOverException(const std::string msg) noexcept;
        ~GameOverException() throw();
        virtual const char *what() const noexcept;
        GameOverException(const GameOverException &) = delete;
        GameOverException &operator=(const GameOverException &) = delete;
    };

  private:
    void movePlayers() noexcept;
    void handleCollisions(const std::chrono::time_point<std::chrono::high_resolution_clock> &now);
    collision_type checkPlayerCollision(const std::shared_ptr<Player> &player,
                                        const std::shared_ptr<Player> &otherPlayer) const noexcept;
    bool checkCollisionPlayerWall(const std::shared_ptr<Player> &player) const noexcept;
    bool checkCollisionPlayerFood(const std::shared_ptr<Player> &player) const noexcept;
    bool checkCollisionPlayerItself(const std::shared_ptr<Player> &player) const noexcept;
    bool checkCollisionOtherPlayer(const std::shared_ptr<Player> &player,
                                   const std::shared_ptr<Player> &otherPlayer) const noexcept;
    void runModesRoutine(const std::chrono::time_point<std::chrono::high_resolution_clock> &now);
    std::shared_ptr<Food> generateFood();
    void handlePlayerDeath(int playerIdx, const std::string str) const;
    void handleGameOver(const std::string str) const;
    // These 2 functions cannot be marked const because of std::uniform_int_distribution works
    const point_t choseRandomUnoccupiedPoint() noexcept;
    const point_t generateRandomPoint() noexcept;
    bool isPointOccupied(const point_t &point) const noexcept;
    void updateScore() noexcept;

    std::string getInfo() const noexcept;
    const std::string collisionToString(collision_type) const noexcept;

  private:
    std::unique_ptr<ModeHandler> _modeHandler;
    int _width;
    int _height;
    size_t _totalFreeSpace;
    long int _gameSpeed;
    double _lastTurn;
    std::chrono::time_point<std::chrono::high_resolution_clock> _clock;
    std::mt19937 _rng;
    std::shared_ptr<Player> _p0;
    std::shared_ptr<Player> _p1;
    std::shared_ptr<Food> _food;
    std::unique_ptr<Score> _scoreHandler;
    bool _shouldPlayEatingSound;

  private:
    static inline constexpr int _DEFAULT_PLAYER_SIZE = 4;
    static inline constexpr long int _DEFAULT_GAME_SPEED_MS = 500000;
};

void checkArgs(int, char **);