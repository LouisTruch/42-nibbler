#pragma once

#include "../../../inc/Interface/IGraphicLib.hpp"
#include "../raylib/src/raylib.h"
#include "RaylibTexture.hpp"
#include <algorithm>
#include <deque>
#include <memory>
#include <string_view>
#include <vector>

class RaylibGraphicLib : public IGraphicLib
{
  public:
    RaylibGraphicLib(int, int);
    ~RaylibGraphicLib();
    RaylibGraphicLib(const RaylibGraphicLib &);
    RaylibGraphicLib &operator=(const RaylibGraphicLib &);
    void clearBoard() const;
    player_input_t getPlayerInput(int) const;
    void resetPlayerInput();
    void registerPlayerInput() noexcept;
    void drawPlayer(const Player &);
    void drawFood(const Food &);
    void drawScores(int, int);

  private:
    RaylibGraphicLib() = delete;
    void drawHead(int, int, int, Color);
    void drawTail(std::deque<point_t>::const_iterator &, Color);
    void drawBody(std::deque<point_t>::const_iterator &, Color);
    // DrawTexture() wrapper to avoid having to add TILE_SIZE to every call;
    void drawTextureTileSize(Texture2D, int, int, Color) const;

  private:
    static constexpr int _TILE_SIZE = 40;
    const Color _boardColor1 = {174, 220, 79, 255};
    const Color _boardColor2 = {146, 206, 63, 255};
    int _width;
    int _height;
    std::vector<RaylibTexture> _vecTexture;
    std::unique_ptr<RaylibTexture> _background;
};

extern "C"
{
    RaylibGraphicLib *makeGraphicLib(int width, int height);
    void destroyGraphicLib(RaylibGraphicLib *gLib);
}