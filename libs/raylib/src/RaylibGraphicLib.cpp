#include "../inc/RaylibGraphicLib.hpp"
#include <cassert>
#include <iostream>
#include <ostream>

RaylibGraphicLib::RaylibGraphicLib(int width, int height) : _width(width * TILE_SIZE), _height(height * TILE_SIZE)
{
    InitWindow(_width, _height, "Raylib Nibbler");
    for (int idx = RaylibTexture::BODY_BOTTOMLEFT; idx != RaylibTexture::TEXTURE_NB_ITEMS; idx++)
        _vecTexture.push_back(static_cast<RaylibTexture::texture_name_e>(idx));
}

void RaylibGraphicLib::drawPlayer(const Player &player)
{
    ClearBackground(BLACK);
    BeginDrawing();
    switch (player._currentDir)
    {
    case UP:
        DrawTexture(_vecTexture[RaylibTexture::HEAD_UP].getTexture(), player._body.front().x * TILE_SIZE,
                    player._body.front().y * TILE_SIZE, WHITE);
        break;
    case DOWN:
        DrawTexture(_vecTexture[RaylibTexture::HEAD_DOWN].getTexture(), player._body.front().x * TILE_SIZE,
                    player._body.front().y * TILE_SIZE, WHITE);
        break;
    case LEFT:
        DrawTexture(_vecTexture[RaylibTexture::HEAD_LEFT].getTexture(), player._body.front().x * TILE_SIZE,
                    player._body.front().y * TILE_SIZE, WHITE);
        break;
    case RIGHT:
        DrawTexture(_vecTexture[RaylibTexture::HEAD_RIGHT].getTexture(), player._body.front().x * TILE_SIZE,
                    player._body.front().y * TILE_SIZE, WHITE);
        break;
    default:
        assert(!"Invalid direction ");
        break;
    }
    for (auto &point : player._body)
    {
        if (point == player._body.front())
            continue;
            
    }
    EndDrawing();
}

void RaylibGraphicLib::drawFood(const point_t &point)
{
    DrawTexture(_vecTexture[RaylibTexture::FOOD].getTexture(), point.x * TILE_SIZE, point.y * TILE_SIZE, WHITE);
}

void RaylibGraphicLib::registerPlayerInput()
{
    if (IsKeyDown(KEY_W))
        _playerInput = UP;
    else if (IsKeyDown(KEY_S))
        _playerInput = DOWN;
    else if (IsKeyDown(KEY_A))
        _playerInput = LEFT;
    else if (IsKeyDown(KEY_D))
        _playerInput = RIGHT;
    else if (IsKeyDown(KEY_ESCAPE))
        _playerInput = QUIT;
    else if (IsKeyDown(KEY_ONE))
        _playerInput = SWAP_LIBNCURSES;
    else if (IsKeyDown(KEY_TWO))
        _playerInput = SWAP_LIBSDL;
}

player_input_t RaylibGraphicLib::getPlayerInput() const
{
    return _playerInput;
}

void RaylibGraphicLib::resetPlayerInput()
{
    _playerInput = DEFAULT;
}

RaylibGraphicLib::~RaylibGraphicLib()
{
    _vecTexture.clear();
    CloseWindow();
}

RaylibGraphicLib::RaylibGraphicLib(const RaylibGraphicLib &other)
{
    *this = other;
}

RaylibGraphicLib &RaylibGraphicLib::operator=(const RaylibGraphicLib &other)
{
    if (&other == this)
        return *this;
    return *this;
}

std::unique_ptr<RaylibGraphicLib> makeGraphicLib(int width, int height)
{
    return std::make_unique<RaylibGraphicLib>(width, height);
}

void destroyGraphicLib(std::unique_ptr<RaylibGraphicLib> gLib)
{
    gLib.reset();
}