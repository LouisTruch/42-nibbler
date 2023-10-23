#include "../inc/SDLGraphicHandler.hpp"
#include <stdexcept>

SDLGraphicLib::SDLGraphicLib(int width, int height)
{
    _window = SDL_CreateWindow("nibbler", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width * TILE_SIZE + (TILE_SIZE * 2), height * TILE_SIZE + (TILE_SIZE * 2), 0);

    if (!_window)
        throw std::runtime_error("Error creating SDL Window");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
        throw std::runtime_error("Error creating SDL Renderer");

    _borders[HORIZONTAL_TOP].h = TILE_SIZE;
    _borders[HORIZONTAL_TOP].w = width * TILE_SIZE + (TILE_SIZE * 2);
    _borders[HORIZONTAL_TOP].x = 0;
    _borders[HORIZONTAL_TOP].y = 0;
    _borders[HORIZONTAL_BOTTOM].h = TILE_SIZE;
    _borders[HORIZONTAL_BOTTOM].w = width * TILE_SIZE + (TILE_SIZE * 2);
    _borders[HORIZONTAL_BOTTOM].x = 0;
    _borders[HORIZONTAL_BOTTOM].y = height * TILE_SIZE + TILE_SIZE;
    _borders[VERTICAL_RIGHT].h = height * TILE_SIZE + TILE_SIZE;
    _borders[VERTICAL_RIGHT].w = TILE_SIZE;
    _borders[VERTICAL_RIGHT].x = 0;
    _borders[VERTICAL_RIGHT].y = 0;
    _borders[VERTICAL_LEFT].h = height * TILE_SIZE + TILE_SIZE;
    _borders[VERTICAL_LEFT].w = TILE_SIZE;
    _borders[VERTICAL_LEFT].x = width * TILE_SIZE + TILE_SIZE;
    _borders[VERTICAL_LEFT].y = 0;
}

void SDLGraphicLib::drawPlayer(const body_t &body)
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 0);
    SDL_RenderFillRect(_renderer, &_borders[HORIZONTAL_TOP]);
    SDL_RenderFillRect(_renderer, &_borders[HORIZONTAL_BOTTOM]);
    SDL_RenderFillRect(_renderer, &_borders[VERTICAL_RIGHT]);
    SDL_RenderFillRect(_renderer, &_borders[VERTICAL_LEFT]);

    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
    for (auto &point : body)
    {
        _rect.x = point.x * TILE_SIZE;
        _rect.y = point.y * TILE_SIZE;
        SDL_RenderFillRect(_renderer, &_rect);
    }
}

void SDLGraphicLib::drawFood(const point_t &point)
{
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 0);
    _rect.x = point.x * TILE_SIZE;
    _rect.y = point.y * TILE_SIZE;
    SDL_RenderFillRect(_renderer, &_rect);
    SDL_RenderPresent(_renderer);
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