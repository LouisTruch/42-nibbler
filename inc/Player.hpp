#pragma once
#include <deque>
#include "types.hpp"

class Player
{
public:
    Player(int, int, int);
    ~Player();
    Player(const Player &);
    Player &operator=(const Player &);
    void setDirection(player_input_t);
    void move(player_input_t);
    void grow();

public:
    body_t body;

private:
    Player();

private:
    int _currentDir;
};