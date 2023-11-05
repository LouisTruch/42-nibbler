#pragma once
#include "types.hpp"
#include <bits/types/clock_t.h>
#include <deque>

class Player
{
  public:
    Player(int, int, int);
    ~Player();
    Player(const Player &);
    Player &operator=(const Player &);
    void setDirection(player_input_t);
    void move(player_input_t);
    void growBody();
    body_t getBody() const;
    typedef body_t::const_iterator it_body;
    it_body getHead() const;
    it_body getTail() const;
    int getPlayerIdx() const;
    int getPlayerScore() const;
    int _currentDir;
    body_t _body;
    int _playerIdx;
    player_state_e getPlayerCollision() const;
    void setPlayerCollision(player_state_e);
    clock_t getHungerTimer() const;
    void setHungerTimer(clock_t);

  private:
    Player() = delete;
    player_state_e _collision;
    clock_t _hungerTimer;

  private:
    inline static int nbPlayer = 0;
};
