#include "../inc/Player.hpp"

void Player::setDirection(player_input_t input)
{
    if (_currentDir + input != 0)
        _currentDir = input;
}

void Player::move(player_input_t input)
{
    if (input >= LEFT && input <= RIGHT)
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

void Player::growBody()
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
