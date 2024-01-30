#include "../../../inc/Game/Player/Player.hpp"
#include "../../../inc/Log/Logger.hpp"

Player::Player(std::size_t idx, int x, int y, int size)
    : _idx(idx), _body({x, y}, size), _nextDirection(DIRECTION_UP), _prevDirection(DIRECTION_UP)
{
    LOG_DEBUG("Player constructor idx:" << idx << " x:" << x << " y:" << y << " size:" << size);
}

Player::~Player()
{
    LOG_DEBUG("Player destructor");
}

Player::Player(const Player &other)
    : _body(other._body), _nextDirection(other._nextDirection), _prevDirection(other._prevDirection)
{
    LOG_DEBUG("Player copy constructor");
}

Player &Player::operator=(const Player &other)
{
    LOG_DEBUG("Player operator=");
    if (this == &other)
        return *this;
    _body = other._body;
    _nextDirection = other._nextDirection;
    _prevDirection = other._prevDirection;
    return *this;
}

void Player::move() noexcept
{
    _prevDirection = _nextDirection;
    _body.move((Body::player_direction)_nextDirection);
}

bool Player::doesPointIntersectP(const point_t &point) const noexcept
{
    return _body.doesPointIntersectP(point);
}

void Player::growBody() noexcept
{
    _body.grow();
}

void Player::setNextDirection(player_input_t input)
{
    _nextDirection = playerInputToPlayerDirection(input);
}

player_direction Player::getNextDirection() const
{
    return _nextDirection;
}

player_direction Player::getPrevDirection() const
{
    return _prevDirection;
}

player_direction Player::playerInputToPlayerDirection(player_input_t input) noexcept
{
    switch (input)
    {
    case INPUT_UP:
        return DIRECTION_UP;
    case INPUT_DOWN:
        return DIRECTION_DOWN;
    case INPUT_LEFT:
        return DIRECTION_LEFT;
    case INPUT_RIGHT:
        return DIRECTION_RIGHT;
    default:
        return NO_DIRECTION;
    }
}

// Old Player class
// void Player::setDirection(player_input_t input)
// {
//     if (input == 0 || input > 2 || input < -2)
//         return;
//     if (_currentDir + input != 0)
//         _currentDir = input;
// }

// void Player::move()
// {
//     switch (_currentDir)
//     {
//     case INPUT_UP:
//         _body.push_front({_body.front().x, _body.front().y - 1});
//         _body.pop_back();
//         break;
//     case INPUT_DOWN:
//         _body.push_front({_body.front().x, _body.front().y + 1});
//         _body.pop_back();
//         break;
//     case INPUT_LEFT:
//         _body.push_front({_body.front().x - 1, _body.front().y});
//         _body.pop_back();
//         break;
//     case INPUT_RIGHT:
//         _body.push_front({_body.front().x + 1, _body.front().y});
//         _body.pop_back();
//         break;
//     }
// }

// void Player::growBody()
// {
//     _body.push_back({_body.back().x, _body.back().y});
// }

// body_t Player::getBody() const
// {
//     return _body;
// }

// Player::it_body Player::getHead() const
// {
//     return _body.begin();
// }

// Player::it_body Player::getTail() const
// {
//     return _body.end();
// }

// int Player::getPlayerScore() const
// {
//     return (int)_body.size();
// }

// int Player::getPlayerIdx() const
// {
//     return _playerIdx;
// }

// Player::Player(int x, int y, int size)
// {
//     _currentDir = INPUT_UP;
//     for (; size; size--)
//         _body.push_back({x, y});
//     _playerIdx = nbPlayer++;
//     _collision = STATE_NOTHING;
//     _hungerTimer = std::clock();
// }

// Player::Player(int playerIdx, const body_t &playerBody, int dir)
// {
//     _body = playerBody;
//     _currentDir = dir;
//     _playerIdx = playerIdx;
// }

// Player::~Player()
// {
// }

// Player::Player(const Player &other)
// {
//     *this = other;
// }

// Player &Player::operator=(const Player &other)
// {
//     if (&other == this)
//         return *this;
//     _body = other._body;
//     return *this;
// }

// player_state_e Player::getPlayerCollision() const
// {
//     return _collision;
// }

// void Player::setPlayerCollision(player_state_e coll)
// {
//     _collision = coll;
// }

// clock_t Player::getHungerTimer() const
// {
//     return _hungerTimer;
// }

// void Player::setHungerTimer(clock_t timer)
// {
//     _hungerTimer = timer;
// }

// int Player::getCurrentDir() const
// {
//     return _currentDir;
// }