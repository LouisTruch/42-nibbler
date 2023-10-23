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
    void setDir(player_input_t);
    void move(player_input_t);

public:
    typedef struct
    {
        int x;
        int y;
    } point_t;
    std::deque<point_t> body;

private:
    Player();

private:
    int _currentDir;
    // enum direction
    // {
    //     UP,
    //     DOWN,
    //     LEFT,
    //     RIGHT,
    // };
};