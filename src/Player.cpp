#include "../inc/Player.hpp"

void Player::setDirection(player_input_t input)
{
    if (input == UP && _currentDir != DOWN)
        _currentDir = UP;
    else if (input == DOWN && _currentDir != UP)
        _currentDir = DOWN;
    else if (input == LEFT && _currentDir != RIGHT)
        _currentDir = LEFT;
    else if (input == RIGHT && _currentDir != LEFT)
        _currentDir = RIGHT;
}

void Player::move(player_input_t input)
{
    setDirection(input);
    switch (_currentDir)
    {
    case UP:
        body.push_front({body.front().x, body.front().y - 1});
        body.pop_back();
        break;
    case DOWN:
        body.push_front({body.front().x, body.front().y + 1});
        body.pop_back();
        break;
    case LEFT:
        body.push_front({body.front().x - 1, body.front().y});
        body.pop_back();
        break;
    case RIGHT:
        body.push_front({body.front().x + 1, body.front().y});
        body.pop_back();
        break;
    }
}

void Player::grow()
{
    body.push_back({body.back().x, body.back().y});
}

Player::Player(int mapWidth, int mapHeight, int size)
{
    _currentDir = UP;
    (void)mapWidth;
    for (; size; size--)
        body.push_back({1, mapHeight - 1});
}

Player::~Player()
{
}

Player::Player(const Player &other)
{
    *this = other;
}

Player &Player::operator=(const Player &other)
{
    if (&other == this)
        return *this;
    body = other.body;
    return *this;
}