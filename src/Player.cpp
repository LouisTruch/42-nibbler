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
        _body.push_front({_body.front().x, _body.front().y - 1});
        _body.pop_back();
        break;
    case DOWN:
        _body.push_front({_body.front().x, _body.front().y + 1});
        _body.pop_back();
        break;
    case LEFT:
        _body.push_front({_body.front().x - 1, _body.front().y});
        _body.pop_back();
        break;
    case RIGHT:
        _body.push_front({_body.front().x + 1, _body.front().y});
        _body.pop_back();
        break;
    }
}

void Player::growBody()
{
    _body.push_back({_body.back().x, _body.back().y});
}

body_t Player::getBody() const
{
    return _body;
}

Player::it_body Player::getHead() const
{
    return _body.begin();
}

Player::it_body Player::getTail() const
{
    return _body.end();
}

Player::Player(int mapWidth, int mapHeight, int size)
{
    _currentDir = UP;
    (void)mapWidth;
    for (; size; size--)
        _body.push_back({1, mapHeight - 1});
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
    _body = other._body;
    return *this;
}
