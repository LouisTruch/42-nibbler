#include "../inc/RaylibGraphicLib.hpp"

RaylibGraphicLib::RaylibGraphicLib(int width, int height) : _width(width * TILE_SIZE), _height(height * TILE_SIZE)
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(_width, _height, "Raylib Nibbler");
    for (int idx = RaylibTexture::BODY_BOTTOMLEFT; idx != RaylibTexture::TEXTURE_NB_ITEMS; idx++)
        _vecTexture.push_back(static_cast<RaylibTexture::texture_name_e>(idx));
    _background = std::make_unique<RaylibTexture>(_width, _height, TILE_SIZE, _boardColor1, _boardColor2);
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
    if (player._playerIdx == 0)
        playerColor = {255, 0, 0, 255};
    else
        playerColor = {255, 255, 0, 255};
    BeginDrawing();
    for (auto current = player._body.begin(); current != player._body.end(); current++)
    {
        if (current == player._body.begin())
            drawHead(player._body.front().x, player._body.front().y, player._currentDir, playerColor);
        else if (current == player._body.end() - 1)
            drawTail(current, playerColor);
        else
            drawBody(current, playerColor);
    }
    EndDrawing();
}

void RaylibGraphicLib::drawHead(int x, int y, int dir, Color playerColor)
{
    switch (dir)
    {
    case UP:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_UP].getTexture(), x, y, playerColor);
        break;
    case DOWN:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_DOWN].getTexture(), x, y, playerColor);
        break;
    case LEFT:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_LEFT].getTexture(), x, y, playerColor);
        break;
    case RIGHT:
        drawTextureTileSize(_vecTexture[RaylibTexture::HEAD_RIGHT].getTexture(), x, y, playerColor);
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
    BeginDrawing();
    drawTextureTileSize(_vecTexture[RaylibTexture::FOOD].getTexture(), point.x, point.y, WHITE);
    EndDrawing();
}

void RaylibGraphicLib::drawScores(int score, int highScore)
{
    BeginDrawing();
    DrawText(TextFormat("Score: %i", score), TILE_SIZE, 0, 16, RED);
    DrawText(TextFormat("HiScore: %i", highScore), TILE_SIZE, 18, 16, GREEN);
    EndDrawing();
}

void RaylibGraphicLib::drawTextureTileSize(Texture2D texture, int x, int y, Color color) const
{
    DrawTexture(texture, x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE, color);
}

void RaylibGraphicLib::registerPlayerInput()
{
    if (IsKeyDown(KEY_W))
        _arrayPlayerInput[0] = UP;
    if (IsKeyDown(KEY_S))
        _arrayPlayerInput[0] = DOWN;
    if (IsKeyDown(KEY_A))
        _arrayPlayerInput[0] = LEFT;
    if (IsKeyDown(KEY_D))
        _arrayPlayerInput[0] = RIGHT;
    if (IsKeyDown(KEY_UP))
        _arrayPlayerInput[1] = UP;
    if (IsKeyDown(KEY_DOWN))
        _arrayPlayerInput[1] = DOWN;
    if (IsKeyDown(KEY_LEFT))
        _arrayPlayerInput[1] = LEFT;
    if (IsKeyDown(KEY_RIGHT))
        _arrayPlayerInput[1] = RIGHT;
    if (IsKeyDown(KEY_ESCAPE))
        _arrayPlayerInput[0] = QUIT;
    else if (IsKeyDown(KEY_ONE))
        _arrayPlayerInput[0] = SWAP_LIBNCURSES;
    else if (IsKeyDown(KEY_TWO))
        _arrayPlayerInput[0] = SWAP_LIBSDL;
}

player_input_t RaylibGraphicLib::getPlayerInput(int playerIdx) const
{
    return _arrayPlayerInput[playerIdx];
}

void RaylibGraphicLib::resetPlayerInput()
{
    _arrayPlayerInput[0] = DEFAULT;
    _arrayPlayerInput[1] = DEFAULT;
}

RaylibGraphicLib::~RaylibGraphicLib()
{
    _vecTexture.clear();
    _background.reset();
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