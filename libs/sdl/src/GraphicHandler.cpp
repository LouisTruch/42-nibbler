#include "../inc/GraphicHandler.hpp"
#include <stdexcept>

GraphicHandler::GraphicHandler(int width, int height)
{
    _window = SDL_CreateWindow("nibbler", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width, height, 0);

    if (!_window)
        throw std::runtime_error("Error creating SDL Window");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
        throw std::runtime_error("Error creating SDL Renderer");
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

GraphicHandler::GraphicHandler()
{
}

GraphicHandler::~GraphicHandler()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

GraphicHandler::GraphicHandler(const GraphicHandler &other)
{
    (void)other;
}

GraphicHandler &GraphicHandler::operator=(const GraphicHandler &other)
{
    (void)other;
    return *this;
}

GraphicHandler *makeGraphicHandler(int width, int height)
{
    return new GraphicHandler(width, height);
}

// void deleteGraphicHandler(void *ptr)
// {
//     SDL_DestroyWindow((SDL_WINDOW *)ptr);
//     // Needed ?
//     // SDL_Quit();
// }