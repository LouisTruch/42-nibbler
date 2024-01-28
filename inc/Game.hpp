#pragma once
#include "./Mode/ModeHandler.hpp"
#include "./Player/Player.hpp"
#include "Food.hpp"
#include "Score.hpp"
#include "types.hpp"
#include <random> // std::mt19937

class Game
{
  public:
    enum collision_type
    {
        NO_COLLISION,
        COLLISION_P0_FOOD,
        COLLISION_P0_ITSELF,
        COLLISION_P0_WALL,
    };

  public:
    Game(board_size_t boardSize, std::unique_ptr<ModeHandler>);
    ~Game();
    void playTurn();
    void setShouldPlayEatingSound(bool) noexcept;
    bool getShouldPlayEatingSound() const noexcept;

    std::shared_ptr<Player> getP0();
    // std::shared_ptr<Player> getP1();
    std::shared_ptr<Food> getFood();

  private:
    Game() = delete;
    void movePlayers() noexcept;
    collision_type checkPlayerCollision(const std::shared_ptr<Player> &player) const noexcept;
    bool checkCollisionPlayerWall(const std::shared_ptr<Player> &player) const noexcept;
    bool checkCollisionPlayerFood(const std::shared_ptr<Player> &player) const noexcept;
    bool checkCollisionPlayerItself(const std::shared_ptr<Player> &player) const noexcept;
    void runModesRoutine(const std::clock_t &now);
    std::shared_ptr<Food> generateFood();
    // These 2 functions cannot be marked const because of std::uniform_int_distribution works
    const point_t choseRandomUnoccupiedPoint() noexcept;
    const point_t generateRandomPoint() noexcept;
    bool isPointOccupied(const point_t &point) const noexcept;
    void updateScore() noexcept;

  private:
    std::unique_ptr<ModeHandler> _modeHandler;
    int _width;
    int _height;
    size_t _totalFreeSpace;
    size_t _gameSpeed;
    double _lastTurn;
    std::mt19937 _rng;
    std::shared_ptr<Player> _p0;
    std::shared_ptr<Player> _p1;
    std::shared_ptr<Food> _food;
    std::unique_ptr<Score> _scoreHandler;
    bool _shouldPlayEatingSound;

  private:
    static inline constexpr int _MIN_WIDTH = 5;
    static inline constexpr int _MAX_WIDTH = 47;
    static inline constexpr int _MIN_HEIGHT = 5;
    static inline constexpr int _MAX_HEIGHT = 24;

    static inline constexpr int _DEFAULT_PLAYER_SIZE = 4;
    static inline constexpr size_t _DEFAULT_GAME_SPEED_MS = 500000;
    // Old Game class
    // Game(const Game &);
    // Game &operator=(const Game &);

    // void initPlayer();
    // void loop();
    // void playersAction(player_action_e);
    // void checkCollisions() const;
    // void checkPlayerState();
    // int handleLibSwitch();
    // point_t chooseRandomFoodPos();
    // point_t generateRandomPoint();
    // bool isTileFree(point_t);
    // void throwGameOverScore(std::string_view) const;
    // double getGameSpeed() const;
    // void setGameSpeed(double);
    // std::string constructGameData() const;
    // int handleMultiplayerInput();
    // void handleGameOver(std::string, int);

    // private:
    //   std::unique_ptr<ModeHandler> _modeHandler;
    //   std::unique_ptr<LibHandler> _libHandler;
    //   std::unique_ptr<IGraphicLib> _graphicHandler;
    //   std::array<std::unique_ptr<Player>, 2> _arrayPlayer;
    //   std::unique_ptr<Food> _food;
    //   std::mt19937 _rng;
    //   int _width;
    //   int _height;
    //   int _totalSpace;
    //   double _gameSpeed;
    //   double _turn;
    //   clock_t _turnStart;

    // public:
    //   class GameOverException : public std::exception
    //   {
    //     private:
    //       std::string _msg;
    //       inline static constexpr std::string_view PLAYER_SCORE_STR = {"\nPlayer Score: "};
    //       inline static constexpr std::string_view MAP_WIDTH_STR = {"\nMap width: "};
    //       inline static constexpr std::string_view MAP_HEIGHT_STR = {"\nMap height: "};

    //     public:
    //       GameOverException(std::string_view, int, int, int, Score *);
    //       virtual const char *what() const throw();
    //       ~GameOverException() throw() = default;
    //   };
};

void checkArgs(int, char **);
