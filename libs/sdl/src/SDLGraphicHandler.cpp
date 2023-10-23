#include "../inc/SDLGraphicHandler.hpp"
#include <stdexcept>

SDLGraphicLib::SDLGraphicLib(int width, int height)
{
    _window = SDL_CreateWindow("nibbler", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width, height, 0);

    if (!_window)
        throw std::runtime_error("Error creating SDL Window");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
        throw std::runtime_error("Error creating SDL Renderer");
}

void SDLGraphicLib::drawMap(const map_t &map)
{
    for (auto &&row : map)
    {
        for (auto &&col : row)
        {
            std::cout << col;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void SDLGraphicLib::getPlayerInput()
{
    while (SDL_PollEvent(&_event) > 0)
    {
        switch (_event.type)
        {
        case SDL_KEYDOWN:
            if (_event.key.keysym.scancode == SDL_SCANCODE_W)
                playerInput = UP;
            else if (_event.key.keysym.scancode == SDL_SCANCODE_S)
                playerInput = DOWN;
            else if (_event.key.keysym.scancode == SDL_SCANCODE_A)
                playerInput = LEFT;
            else if (_event.key.keysym.scancode == SDL_SCANCODE_D)
                playerInput = RIGHT;
            else if (_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                playerInput = QUIT;
            break;
        }
    }
}

SDLGraphicLib::SDLGraphicLib()
{
}

SDLGraphicLib::~SDLGraphicLib()
{
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

std::unique_ptr<SDLGraphicLib> makeGraphicLib(int width, int height)
{
    return std::make_unique<SDLGraphicLib>(width, height);
}

void destroyGraphicLib(std::unique_ptr<SDLGraphicLib> gLib)
{
    gLib.reset();
}

// SDLGraphicLib *makeGraphicHandler(int width, int height)
// {
//     return new SDLGraphicLib(width, height);
// }

// void deleteGraphicHandler(void *ptr)
// {
//     SDL_DestroyWindow((SDL_WINDOW *)ptr);
//     // Needed ?
//     // SDL_Quit();
// }