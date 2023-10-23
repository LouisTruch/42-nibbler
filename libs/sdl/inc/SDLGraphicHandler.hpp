#pragma once

#include <stddef.h>
#include <SDL2/SDL.h>
#include <memory>

#include "../../../inc/IGraphicLib.hpp"

constexpr int TILE_SIZE = 5;

class SDLGraphicLib : public IGraphicLib
{
public:
    SDLGraphicLib(int, int);
    ~SDLGraphicLib();
    SDLGraphicLib(const SDLGraphicLib &);
    SDLGraphicLib &operator=(const SDLGraphicLib &);
    void drawMap(const map_t &map);
    void getPlayerInput();

private:
    SDLGraphicLib();

private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Event _event;
    SDL_Rect _rect = {0, 0, TILE_SIZE, TILE_SIZE};
};

extern "C"
{
    std::unique_ptr<SDLGraphicLib> makeGraphicLib(int width, int height);
    void destroyGraphicLib(std::unique_ptr<SDLGraphicLib> gLib);
    // SDLGraphicLib *makeGraphicHandler(int width, int height);
    // void destroyGraphicHandler();
}