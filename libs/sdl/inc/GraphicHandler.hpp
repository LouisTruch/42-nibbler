#pragma once

#include <stddef.h>
#include <SDL2/SDL.h>
#include <memory>

#include "../../../inc/IGraphicHandler.hpp"

class GraphicHandler : public IGraphicHandler
{
public:
    GraphicHandler(int, int);
    ~GraphicHandler();
    GraphicHandler(const GraphicHandler &);
    GraphicHandler &operator=(const GraphicHandler &);

private:
    GraphicHandler();
    SDL_Window *_window;
    SDL_Renderer *_renderer;
};

extern "C"
{
    GraphicHandler *makeGraphicHandler(int width, int height);
    // void deleteGraphicHandler(void *ptr);
}