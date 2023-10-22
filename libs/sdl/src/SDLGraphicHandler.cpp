#include "../inc/SDLGraphicHandler.hpp"
#include <stdexcept>

#include <iostream>
SDLGraphicLib::SDLGraphicLib(int width, int height)
{
    std::cout << "SDL Lib Created\n";
    _window = SDL_CreateWindow("nibbler", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width, height, 0);

    if (!_window)
        throw std::runtime_error("Error creating SDL Window");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
        throw std::runtime_error("Error creating SDL Renderer");
}

SDLGraphicLib::SDLGraphicLib()
{
}

SDLGraphicLib::~SDLGraphicLib()
{
    std::cout << "SDL Lib Destroyed\n";
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

SDLGraphicLib::SDLGraphicLib(const SDLGraphicLib &other)
{
    (void)other;
}

SDLGraphicLib &SDLGraphicLib::operator=(const SDLGraphicLib &other)
{
    (void)other;
    return *this;
}

SDLGraphicLib *makeGraphicHandler(int width, int height)
{
    return new SDLGraphicLib(width, height);
}

std::unique_ptr<SDLGraphicLib> makeGraphicLib(int width, int height)
{
    return std::make_unique<SDLGraphicLib>(width, height);
}

void destroyGraphicLib(std::unique_ptr<SDLGraphicLib> gLib)
{
    gLib.reset();
}

// void deleteGraphicHandler(void *ptr)
// {
//     SDL_DestroyWindow((SDL_WINDOW *)ptr);
//     // Needed ?
//     // SDL_Quit();
// }