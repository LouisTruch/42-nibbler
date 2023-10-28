#pragma once
#include "types.hpp"
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
    int _currentDir;
    body_t _body;

  private:
    Player() = delete;

  private:
};
