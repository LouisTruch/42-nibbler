#pragma once

#include <stddef.h>
#include <SDL2/SDL.h>
#include <memory>

#include "../../../inc/IGraphicLib.hpp"

class SDLGraphicLib : public IGraphicLib
{
public:
    SDLGraphicLib(int, int);
    ~SDLGraphicLib();
    SDLGraphicLib(const SDLGraphicLib &);
    SDLGraphicLib &operator=(const SDLGraphicLib &);

private:
    SDLGraphicLib();
    SDL_Window *_window;
    SDL_Renderer *_renderer;
};

extern "C"
{
    SDLGraphicLib *makeGraphicHandler(int width, int height);
    std::unique_ptr<SDLGraphicLib> makeGraphicLib(int width, int height);
    void destroyGraphicLib(std::unique_ptr<SDLGraphicLib> gLib);
    // void destroyGraphicHandler();
}