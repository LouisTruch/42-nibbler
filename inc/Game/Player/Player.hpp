#pragma once

#include "../../types.hpp"
#include "Body.hpp"
#include <bits/types/clock_t.h>
#include <deque>

class Player
{
  public:
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
    player_direction _nextDirection;
    player_direction _prevDirection;

  private:
    Player() = delete;

    player_direction playerInputToPlayerDirection(player_input_t input) noexcept;
};
