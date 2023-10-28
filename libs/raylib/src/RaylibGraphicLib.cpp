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
    // for (int i = 0; i < _width; i++)
    // {
    //     if (i % 2)
    //         for (int j = 0; j < _height; j++)
    //         {
    //             if (j % 2)
    //                 DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
    //             else
    //                 DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIME);
    //         }
    //     else
    //         for (int j = 0; j < _height; j++)
    //         {
    //             if (j % 2)
    //                 DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, LIME);
    //             else
    //                 DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
    //         }
    // }

    BeginDrawing();
    ClearBackground(BLACK);
    for (auto current = player._body.begin(); current != player._body.end(); current++)
    {
        if (current == player._body.begin())
        {
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
            continue;
        }
        else if (current == player._body.end() - 1)
        {
            // Fix on spawning + eating
            if (current->x == (current - 1)->x)
            {
                if (current->y > (current - 1)->y)
                    DrawTexture(_vecTexture[RaylibTexture::TAIL_DOWN].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else
                    DrawTexture(_vecTexture[RaylibTexture::TAIL_UP].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
            }
            else
            {
                if (current->x > (current - 1)->x)
                    DrawTexture(_vecTexture[RaylibTexture::TAIL_RIGHT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else
                    DrawTexture(_vecTexture[RaylibTexture::TAIL_LEFT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
            }
        }
        else
        {
            auto prev = current - 1;
            auto next = current + 1;
            if (current->x == next->x && prev->y == current->y)
            {
                if (prev->x > current->x && current->y < next->y)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMRIGHT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else if (prev->x < current->x && current->y < next->y)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMLEFT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else if (prev->x > current->x && current->y > next->y)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_TOPRIGHT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else if (prev->x < current->x && current->y > next->y)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_TOPLEFT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
            }
            else if (current->y == next->y && prev->x == current->x)
            {
                if (prev->y > current->y && current->x < next->x)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMRIGHT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else if (prev->y < current->y && current->x < next->x)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_TOPRIGHT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else if (prev->y > current->y && current->x > next->x)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMLEFT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
                else if (prev->y < current->y && current->x > next->x)
                    DrawTexture(_vecTexture[RaylibTexture::BODY_TOPLEFT].getTexture(), current->x * TILE_SIZE,
                                current->y * TILE_SIZE, WHITE);
            }
            else if ((current - 1)->x == current->x)
                DrawTexture(_vecTexture[RaylibTexture::BODY_VERTICAL].getTexture(), current->x * TILE_SIZE,
                            current->y * TILE_SIZE, WHITE);
            else if ((current - 1)->y == current->y)
                DrawTexture(_vecTexture[RaylibTexture::BODY_HORIZONTAL].getTexture(), current->x * TILE_SIZE,
                            current->y * TILE_SIZE, WHITE);
        }
    }
}

// void    draw_eeeeeeeeee(?? a b c d eeee)
// {
//     if (a > b && b < c)
//         DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMRIGHT].getTexture(), it_body->x * TILE_SIZE,
//                     it_body->y * TILE_SIZE, WHITE);
//     else if (a < b && b > c)
//         DrawTexture(_vecTexture[RaylibTexture::BODY_TOPLEFT].getTexture(), it_body->x * TILE_SIZE,
//                     it_body->y * TILE_SIZE, WHITE);
//     else if (a < b && b < c || a > b && b > c && eeee = 0)
//         DrawTexture(_vecTexture[RaylibTexture::BODY_TOPRIGHT].getTexture(), it_body->x * TILE_SIZE,
//                     it_body->y * TILE_SIZE, WHITE);
//     else
//         DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMLEFT].getTexture(), it_body->x * TILE_SIZE,
//                     it_body->y * TILE_SIZE, WHITE);
// }

void RaylibGraphicLib::drawFood(const point_t &point)
{
    DrawTexture(_vecTexture[RaylibTexture::FOOD].getTexture(), point.x * TILE_SIZE, point.y * TILE_SIZE, WHITE);
    EndDrawing();
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