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
    DrawTexture(_background->getTexture(), _TILE_SIZE, _TILE_SIZE, WHITE);
}

void RaylibGraphicLib::drawPlayer(const Snake_t &player)
{
    Color playerColor;
    if (player.idx == 0)
        playerColor = {255, 0, 0, 255};
    else
        playerColor = {255, 255, 0, 255};

    for (auto current = player.body.begin(); current != player.body.end(); current++)
    {
        if (current == player.body.begin())
        {
            drawHead(player.body.front().x, player.body.front().y, player.dir, playerColor);
        }
        else if (current == player.body.end() - 1)
        {
            auto oneBeforeTail = getPrevDiffPos(current, player.body.begin());
            if (!(current->x == player.body.front().x && current->y == player.body.front().y))
                drawTail(oneBeforeTail, current, playerColor);
        }
        else
        {
            drawBody(current, playerColor);
        }
    }
}

void RaylibGraphicLib::drawHead(int x, int y, int dir, Color playerColor)
{
    switch (dir)
    {
    case DIRECTION_UP:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_UP].getTexture(), x, y, playerColor);
        break;
    case DIRECTION_DOWN:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_DOWN].getTexture(), x, y, playerColor);
        break;
    case DIRECTION_LEFT:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_LEFT].getTexture(), x, y, playerColor);
        break;
    case DIRECTION_RIGHT:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_RIGHT].getTexture(), x, y, playerColor);
        break;
    default:
        // Should never happen
        break;
    }
}

std::deque<point_t>::const_iterator RaylibGraphicLib::getPrevDiffPos(
    const std::deque<point_t>::const_iterator current, const std::deque<point_t>::const_iterator first) const
{
    auto prev = current;
    while (prev != first)
    {
        prev--;
        if (prev->x != current->x || prev->y != current->y)
            break;
    }
    return prev;
}

void RaylibGraphicLib::drawTail(std::deque<point_t>::const_iterator &oneBeforeTail,
                                std::deque<point_t>::const_iterator &tail, Color playerColor)
{
    // if (oneBeforeTail->x == tail->x && oneBeforeTail->y == tail->y)
    // return;
    if (oneBeforeTail->x == tail->x)
    {
        if (oneBeforeTail->y < tail->y)
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_DOWN].getTexture(), tail->x, tail->y, playerColor);
        else
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_UP].getTexture(), tail->x, tail->y, playerColor);
    }
    else if (oneBeforeTail->y == tail->y)
    {
        if (oneBeforeTail->x < tail->x)
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_RIGHT].getTexture(), tail->x, tail->y, playerColor);
        else
            drawTextureTileSize(_vecTexture[RaylibTexture::TAIL_LEFT].getTexture(), tail->x, tail->y, playerColor);
    }
    // Useful when snake is spawning or eating to avoid printing wrong texture
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

void RaylibGraphicLib::drawFood(const point_t &point)
{
    drawTextureTileSize(_vecTexture[RaylibTexture::FOOD].getTexture(), point.x, point.y, WHITE);
    EndDrawing();
}

void RaylibGraphicLib::drawScores(int score, int highScore)
{
    BeginDrawing();
    DrawText(TextFormat("Score: %i", score), _TILE_SIZE, 0, 16, RED);
    DrawText(TextFormat("HiScore: %i", highScore), _TILE_SIZE, 18, 16, GREEN);
}

void RaylibGraphicLib::drawTextureTileSize(Texture2D texture, int x, int y, Color color) const
{
    DrawTexture(texture, x * _TILE_SIZE, y * _TILE_SIZE, color);
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