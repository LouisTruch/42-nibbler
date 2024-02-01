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