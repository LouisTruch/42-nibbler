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
}

void SDLGraphicLib::drawMap(const map_t &map)
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);
    for (auto itMap = map.begin(); itMap != map.end(); ++itMap)
    {
        int i = std::distance(map.begin(), itMap);
        for (auto itLine = map[i].begin(); itLine != map[i].end(); ++itLine)
        {
            int j = std::distance(map[i].begin(), itLine);
            switch (map[i][j])
            {
            case OBJ_PLAYER:
                _rect.x = i * TILE_SIZE;
                _rect.y = j * TILE_SIZE;
                SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
                SDL_RenderFillRect(_renderer, &_rect);
                /* code */
                break;

            default:
                break;
            }
        }
    }
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