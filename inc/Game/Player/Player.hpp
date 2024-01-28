#pragma once

#include "../../types.hpp"
#include "Body.hpp"
#include <bits/types/clock_t.h>
#include <deque>

class Player
{
  public:
    typedef enum
    {
        DIRECTION_UP = -1,
        DIRECTION_DOWN = 1,
        NO_DIRECTION = 0,
        DIRECTION_LEFT = -2,
        DIRECTION_RIGHT = 2
    } player_direction;

  public:
    Player(std::size_t idx, int x, int y, int size);
    ~Player();
    Player(const Player &);
    Player &operator=(const Player &);

  public:
    void move() noexcept;
    bool doesPointIntersectP(const point_t &point) const noexcept;
    void growBody() noexcept;

  public:
    void setNextDirection(player_input_t input);
    player_direction getNextDirection() const;
    player_direction getPrevDirection() const;

  public:
    // Have to make them public because of symbol lookup error when using getters
    std::size_t _idx;
    Body _body;
    // This one only for Raylib
    player_direction _nextDirection;
    player_direction _prevDirection;

  private:
    Player() = delete;

    player_direction playerInputToPlayerDirection(player_input_t input) noexcept;
    // Old Player class
    // public:
    //   Player(int, int, int);
    //   Player(int, const body_t &, int);
    //   ~Player();
    //   Player(const Player &);
    //   Player &operator=(const Player &);
    //   void setDirection(player_input_t);
    //   void move();
    //   void growBody();
    //   body_t getBody() const;
    //   typedef body_t::const_iterator it_body;
    //   it_body getHead() const;
    //   it_body getTail() const;
    //   int getPlayerIdx() const;
    //   int getPlayerScore() const;
    //   int getCurrentDir() const;
    //   int _currentDir;
    //   body_t _body;
    //   int _playerIdx;
    //   player_state_e getPlayerCollision() const;
    //   void setPlayerCollision(player_state_e);
    //   clock_t getHungerTimer() const;
    //   void setHungerTimer(clock_t);

    // private:
    //   Player() = delete;
    //   player_state_e _collision;
    //   clock_t _hungerTimer;

    // private:
    //   inline static int nbPlayer = 0;
};
