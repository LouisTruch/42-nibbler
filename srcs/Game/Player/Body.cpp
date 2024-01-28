#include "../../../inc/Game/Player/Body.hpp"
#include "../../../inc/Log/Logger.hpp"

Body::Body(point_t spawn, std::size_t size)
{
    LOG_DEBUG("Body constructor size:" << size);
    for (; size; size--)
    {
        _deque.push_back({spawn.x, spawn.y});
    }
}

Body::~Body()
{
    LOG_DEBUG("Body destructor");
}

Body::Body(const Body &other)
{
    LOG_DEBUG("Body copy constructor");
    _deque = other._deque;
}

Body &Body::operator=(const Body &other)
{
    LOG_DEBUG("Body operator=");
    if (this == &other)
        return *this;
    _deque = other._deque;
    return *this;
}

void Body::move(player_direction direction) noexcept
{
    point_t new_head = _deque.front();
    switch (direction)
    {
    case DIRECTION_UP:
        new_head.y--;
        break;
    case DIRECTION_DOWN:
        new_head.y++;
        break;
    case DIRECTION_LEFT:
        new_head.x--;
        break;
    case DIRECTION_RIGHT:
        new_head.x++;
        break;
    default:
        break;
    }
    _deque.push_front(new_head);
    _deque.pop_back();
}

bool Body::doesPointIntersectP(const point_t &point) const noexcept
{
    for (auto it = _deque.begin(); it != _deque.end(); ++it)
    {
        if (it->x == point.x && it->y == point.y)
            return true;
    }
    return false;
}

void Body::grow() noexcept
{
    _deque.push_back(_deque.back());
}