#include "../inc/RaylibGraphicLib.hpp"
#include <memory> // std::make_unique

RaylibGraphicLib::RaylibGraphicLib(int width, int height) : _width(width * _TILE_SIZE), _height(height * _TILE_SIZE)
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(_width, _height, "Raylib Nibbler");
    for (int idx = RaylibTexture::BODY_BOTTOMLEFT; idx != RaylibTexture::TEXTURE_NB_ITEMS; idx++)
        _vecTexture.push_back(static_cast<RaylibTexture::texture_name_e>(idx));
    _background = std::make_unique<RaylibTexture>(_width, _height, _TILE_SIZE, _boardColor1, _boardColor2);
}

void RaylibGraphicLib::clearBoard() const
{
    ClearBackground(BLACK);
    BeginDrawing();
    drawTextureTileSize(_background->getTexture(), 0, 0, WHITE);
}

void RaylibGraphicLib::drawPlayer(const Player &player)
{
    Color playerColor;
    if (player._idx == 0)
        playerColor = {255, 0, 0, 255};
    else if (player._idx == 1)
        playerColor = {255, 255, 0, 255};
    for (auto current = player._body._deque.begin(); current != player._body._deque.end(); current++)
    {
        if (current == player._body._deque.begin())
            drawHead(player._body._deque.front().x, player._body._deque.front().y, player._nextDirection, playerColor);
        else if (current == player._body._deque.end() - 1)
            drawTail(current, playerColor);
        else
            drawBody(current, playerColor);
    }
}

void RaylibGraphicLib::drawHead(int x, int y, int dir, Color playerColor)
{
    switch (dir)
    {
    case Player::DIRECTION_UP:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_UP].getTexture(), x, y, playerColor);
        break;
    case Player::DIRECTION_DOWN:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_DOWN].getTexture(), x, y, playerColor);
        break;
    case Player::DIRECTION_LEFT:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_LEFT].getTexture(), x, y, playerColor);
        break;
    case Player::DIRECTION_RIGHT:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_RIGHT].getTexture(), x, y, playerColor);
        break;
    default:
        // Should never happen
        break;
    }
}

void RaylibGraphicLib::drawTail(std::deque<point_t>::const_iterator &current, Color playerColor)
{
    // Useful when snake is spawning or eating to avoid printing wrong texture
    auto last = current;
    while (*last == *current)
        last--;
    last++;
    if (last->x == (last - 1)->x)
    {
        if (last->y > (last - 1)->y)
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_DOWN].getTexture(), last->x, last->y, playerColor);
        else
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_UP].getTexture(), last->x, last->y, playerColor);
    }
    else
    {
        if (last->x > (last - 1)->x)
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_RIGHT].getTexture(), last->x, last->y, playerColor);
        else
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_LEFT].getTexture(), last->x, last->y, playerColor);
    }
}

void RaylibGraphicLib::drawBody(std::deque<point_t>::const_iterator &current, Color playerColor)
{
    auto prev = current - 1;
    auto next = current + 1;
    if (current->x == next->x && prev->y == current->y)
    {
        if (prev->x > current->x && current->y < next->y)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_BOTTOMRIGHT].getTexture(), current->x, current->y,
                                playerColor);
        else if (prev->x < current->x && current->y < next->y)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_BOTTOMLEFT].getTexture(), current->x, current->y,
                                playerColor);
        else if (prev->x > current->x && current->y > next->y)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_TOPRIGHT].getTexture(), current->x, current->y,
                                playerColor);
        else if (prev->x < current->x && current->y > next->y)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_TOPLEFT].getTexture(), current->x, current->y,
                                playerColor);
    }
    else if (current->y == next->y && prev->x == current->x)
    {
        if (prev->y > current->y && current->x < next->x)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_BOTTOMRIGHT].getTexture(), current->x, current->y,
                                playerColor);
        else if (prev->y < current->y && current->x < next->x)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_TOPRIGHT].getTexture(), current->x, current->y,
                                playerColor);
        else if (prev->y > current->y && current->x > next->x)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_BOTTOMLEFT].getTexture(), current->x, current->y,
                                playerColor);
        else if (prev->y < current->y && current->x > next->x)
            drawTextureTileSize(_vecTexture[RaylibTexture::BODY_TOPLEFT].getTexture(), current->x, current->y,
                                playerColor);
    }
    else if ((current - 1)->x == current->x)
        drawTextureTileSize(_vecTexture[RaylibTexture::BODY_VERTICAL].getTexture(), current->x, current->y,
                            playerColor);
    else if ((current - 1)->y == current->y)
        drawTextureTileSize(_vecTexture[RaylibTexture::BODY_HORIZONTAL].getTexture(), current->x, current->y,
                            playerColor);
}

// void    draw_eeeeeeeeee(?? a b c d eeee)
// {
//     if (a > b && b < c)
//         DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMRIGHT].getTexture(), it_body->x * _TILE_SIZE,
//                     it_body->y * _TILE_SIZE, WHITE);
//     else if (a < b && b > c)
//         DrawTexture(_vecTexture[RaylibTexture::BODY_TOPLEFT].getTexture(), it_body->x * _TILE_SIZE,
//                     it_body->y * _TILE_SIZE, WHITE);
//     else if (a < b && b < c || a > b && b > c && eeee = 0)
//         DrawTexture(_vecTexture[RaylibTexture::BODY_TOPRIGHT].getTexture(), it_body->x * _TILE_SIZE,
//                     it_body->y * _TILE_SIZE, WHITE);
//     else
//         DrawTexture(_vecTexture[RaylibTexture::BODY_BOTTOMLEFT].getTexture(), it_body->x * _TILE_SIZE,
//                     it_body->y * _TILE_SIZE, WHITE);
// }

void RaylibGraphicLib::drawFood(const Food &food)
{
    drawTextureTileSize(_vecTexture[RaylibTexture::FOOD].getTexture(), food._pos.x, food._pos.y, WHITE);
    EndDrawing();
}

void RaylibGraphicLib::drawScores(int score, int highScore)
{
    BeginDrawing();
    DrawText(TextFormat("Score: %i", score), _TILE_SIZE, 0, 16, RED);
    DrawText(TextFormat("HiScore: %i", highScore), _TILE_SIZE, 18, 16, GREEN);
    EndDrawing();
}

void RaylibGraphicLib::drawTextureTileSize(Texture2D texture, int x, int y, Color color) const
{
    DrawTexture(texture, x * _TILE_SIZE + _TILE_SIZE, y * _TILE_SIZE + _TILE_SIZE, color);
}

void RaylibGraphicLib::registerPlayerInput() noexcept
{
    if (IsKeyDown(KEY_W))
        _arrayPlayerInput[0] = INPUT_UP;
    if (IsKeyDown(KEY_S))
        _arrayPlayerInput[0] = INPUT_DOWN;
    if (IsKeyDown(KEY_A))
        _arrayPlayerInput[0] = INPUT_LEFT;
    if (IsKeyDown(KEY_D))
        _arrayPlayerInput[0] = INPUT_RIGHT;
    if (IsKeyDown(KEY_UP))
        _arrayPlayerInput[1] = INPUT_UP;
    if (IsKeyDown(KEY_DOWN))
        _arrayPlayerInput[1] = INPUT_DOWN;
    if (IsKeyDown(KEY_LEFT))
        _arrayPlayerInput[1] = INPUT_LEFT;
    if (IsKeyDown(KEY_RIGHT))
        _arrayPlayerInput[1] = INPUT_RIGHT;
    if (IsKeyDown(KEY_ESCAPE))
        _arrayPlayerInput[0] = INPUT_QUIT;
    else if (IsKeyDown(KEY_ONE))
        _arrayPlayerInput[0] = INPUT_SWAP_LIBNCURSES;
    else if (IsKeyDown(KEY_TWO))
        _arrayPlayerInput[0] = INPUT_SWAP_LIBSDL;
#ifdef DEBUG
    else if (IsKeyDown(KEY_THREE))
        _arrayPlayerInput[0] = INPUT_SWAP_LIBRAYLIB;
    else if (IsKeyDown(KEY_FOUR))
        _arrayPlayerInput[0] = INPUT_SWAP_LIBDEBUG;
#endif
}

player_input_t RaylibGraphicLib::getPlayerInput(int playerIdx) const
{
    return _arrayPlayerInput[playerIdx];
}

void RaylibGraphicLib::resetPlayerInput()
{
    _arrayPlayerInput[0] = INPUT_DEFAULT;
    _arrayPlayerInput[1] = INPUT_DEFAULT;
}

RaylibGraphicLib::~RaylibGraphicLib()
{
    _vecTexture.clear();
    _background.reset();
    CloseWindow();
}

extern "C"
{
    RaylibGraphicLib *makeGraphicLib(int width, int height)
    {
        return new RaylibGraphicLib(width, height);
    }

    void destroyGraphicLib(RaylibGraphicLib *gLib)
    {
        delete gLib;
    }
}