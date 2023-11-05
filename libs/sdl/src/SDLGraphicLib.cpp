#include "../inc/SDLGraphicLib.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <ranges>
#include <stdexcept>

SDLGraphicLib::SDLGraphicLib(int width, int height) : _width(width * TILE_SIZE), _height(height * TILE_SIZE)
{
    SDL_Init(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow("nibbler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, 0);

    if (!_window)
        throw std::runtime_error("Error creating SDL Window");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
        throw std::runtime_error("Error creating SDL Renderer");

    // Set up border walls rectangle
    _borders[HORIZONTAL_TOP].h = TILE_SIZE;
    _borders[HORIZONTAL_TOP].w = _width;
    _borders[HORIZONTAL_TOP].x = 0;
    _borders[HORIZONTAL_TOP].y = 0;
    _borders[HORIZONTAL_BOTTOM].h = TILE_SIZE;
    _borders[HORIZONTAL_BOTTOM].w = _width;
    _borders[HORIZONTAL_BOTTOM].x = 0;
    _borders[HORIZONTAL_BOTTOM].y = _height - TILE_SIZE;
    _borders[VERTICAL_RIGHT].h = _height;
    _borders[VERTICAL_RIGHT].w = TILE_SIZE;
    _borders[VERTICAL_RIGHT].x = 0;
    _borders[VERTICAL_RIGHT].y = 0;
    _borders[VERTICAL_LEFT].h = _height;
    _borders[VERTICAL_LEFT].w = TILE_SIZE;
    _borders[VERTICAL_LEFT].x = _width - TILE_SIZE;
    _borders[VERTICAL_LEFT].y = 0;
}

void SDLGraphicLib::clearBoard() const
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);
}

void SDLGraphicLib::drawPlayer(const Player &player)
{
    // Clear screen and render border walls
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 0);
    for (auto &border : _borders)
        SDL_RenderFillRect(_renderer, &border);

    // Render Player
    if (player._playerIdx == 0)
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
    else
        SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 0);
    for (auto &point : player._body)
    {
        _rect.x = point.x * TILE_SIZE + TILE_SIZE;
        _rect.y = point.y * TILE_SIZE + TILE_SIZE;
        SDL_RenderFillRect(_renderer, &_rect);
        if (point == player._body.front())
        {
            if (player._playerIdx == 0)
                SDL_SetRenderDrawColor(_renderer, 175, 0, 0, 0);
            else
                SDL_SetRenderDrawColor(_renderer, 175, 175, 0, 0);
        }
    }
}

void SDLGraphicLib::drawFood(const point_t &point)
{
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 0);
    _rect.x = point.x * TILE_SIZE + TILE_SIZE;
    _rect.y = point.y * TILE_SIZE + TILE_SIZE;
    SDL_RenderFillRect(_renderer, &_rect);
    SDL_RenderPresent(_renderer);
}

void SDLGraphicLib::drawScores(int score, int highScore)
{
    (void)score;
    (void)highScore;
}

void SDLGraphicLib::registerPlayerInput()
{
    while (SDL_PollEvent(&_event) > 0)
    {
        switch (_event.type)
        {
        case SDL_KEYDOWN:
            if (_event.key.keysym.scancode == SDL_SCANCODE_W)
                _arrayPlayerInput[0] = UP;
            if (_event.key.keysym.scancode == SDL_SCANCODE_S)
                _arrayPlayerInput[0] = DOWN;
            if (_event.key.keysym.scancode == SDL_SCANCODE_A)
                _arrayPlayerInput[0] = LEFT;
            if (_event.key.keysym.scancode == SDL_SCANCODE_D)
                _arrayPlayerInput[0] = RIGHT;
            if (_event.key.keysym.scancode == SDL_SCANCODE_UP)
                _arrayPlayerInput[1] = UP;
            if (_event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                _arrayPlayerInput[1] = DOWN;
            if (_event.key.keysym.scancode == SDL_SCANCODE_LEFT)
                _arrayPlayerInput[1] = LEFT;
            if (_event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
                _arrayPlayerInput[1] = RIGHT;
            else if (_event.key.keysym.scancode == SDL_SCANCODE_1)
                _arrayPlayerInput[0] = SWAP_LIBNCURSES;
            else if (_event.key.keysym.scancode == SDL_SCANCODE_3)
                _arrayPlayerInput[0] = SWAP_LIBRAYLIB;
            else if (_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                _arrayPlayerInput[0] = QUIT;
            break;
        }
    }
}

player_input_t SDLGraphicLib::getPlayerInput(int playerIdx) const
{
    return _arrayPlayerInput[playerIdx];
}

void SDLGraphicLib::resetPlayerInput()
{
    _arrayPlayerInput[0] = DEFAULT;
    _arrayPlayerInput[1] = DEFAULT;
}

SDLGraphicLib::~SDLGraphicLib()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

SDLGraphicLib::SDLGraphicLib(const SDLGraphicLib &other)
{
    *this = other;
}

SDLGraphicLib &SDLGraphicLib::operator=(const SDLGraphicLib &other)
{
    if (&other == this)
        return *this;
    _window = other._window;
    _renderer = other._renderer;
    _event = other._event;
    _rect = other._rect;
    for (int i = 0; auto &border : other._borders)
        _borders[i++] = border;
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