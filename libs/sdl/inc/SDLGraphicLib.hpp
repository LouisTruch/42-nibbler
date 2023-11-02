#pragma once

#include "../../../inc/IGraphicLib.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <stddef.h>

class SDLGraphicLib : public IGraphicLib
{
  public:
    SDLGraphicLib(int, int);
    ~SDLGraphicLib();
    SDLGraphicLib(const SDLGraphicLib &);
    SDLGraphicLib &operator=(const SDLGraphicLib &);
    player_input_t getPlayerInput() const;
    void resetPlayerInput();
    void registerPlayerInput();
    void drawPlayer(const Player &);
    void drawFood(const point_t &);

  private:
    SDLGraphicLib() = delete;

  private:
    int _width;
    int _height;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Event _event;
    SDL_Rect _rect = {0, 0, TILE_SIZE, TILE_SIZE};
    enum screen_borders_e
    {
        HORIZONTAL_TOP,
        HORIZONTAL_BOTTOM,
        VERTICAL_LEFT,
        VERTICAL_RIGHT,
    };
    SDL_Rect _borders[4];
    static constexpr int TILE_SIZE = 10;
};

extern "C"
{
    std::unique_ptr<SDLGraphicLib> makeGraphicLib(int width, int height);
    void destroyGraphicLib(std::unique_ptr<SDLGraphicLib> gLib);
}