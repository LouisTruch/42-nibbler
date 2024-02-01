#pragma once

#include "../../../inc/Interface/IGraphicLib.hpp"

#ifndef MANUAL_INSTALLSDL
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

class SDLGraphicLib : public IGraphicLib
{
  public:
    SDLGraphicLib(int, int);
    ~SDLGraphicLib();

  public:
    void clearBoard() const;
    player_input_t getPlayerInput(int) const;
    void resetPlayerInput();
    void registerPlayerInput() noexcept;
    void drawPlayer(const Snake_t &snake);
    void drawFood(const point_t &point);
    void drawScores(int, int);

  private:
    SDLGraphicLib() = delete;
    SDLGraphicLib(const SDLGraphicLib &) = delete;
    SDLGraphicLib &operator=(const SDLGraphicLib &) = delete;

  private:
    int _width;
    int _height;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Event _event;
    SDL_Rect _rect = {0, 0, _TILE_SIZE, _TILE_SIZE};
    enum screen_borders_e
    {
        HORIZONTAL_TOP,
        HORIZONTAL_BOTTOM,
        VERTICAL_LEFT,
        VERTICAL_RIGHT,
    };
    SDL_Rect _borders[4];
    static constexpr int _TILE_SIZE = 10;
};

extern "C"
{
    SDLGraphicLib *makeGraphicLib(int width, int height);
    void destroyGraphicLib(SDLGraphicLib *gLib);
}