#include "../inc/SDLGraphicLib.hpp"
#include <iostream>
#include <ranges>
#include <stdexcept>

#ifndef MANUAL_INSTALL
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

SDLGraphicLib::SDLGraphicLib(int width, int height) : _width(width * _TILE_SIZE), _height(height * _TILE_SIZE)
{
    SDL_Init(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow("nibbler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, 0);

    if (!_window)
        throw std::runtime_error("Error creating SDL Window");

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
        throw std::runtime_error("Error creating SDL Renderer");

    // Set up border walls rectangle
    _borders[HORIZONTAL_TOP].h = _TILE_SIZE;
    _borders[HORIZONTAL_TOP].w = _width;
    _borders[HORIZONTAL_TOP].x = 0;
    _borders[HORIZONTAL_TOP].y = 0;
    _borders[HORIZONTAL_BOTTOM].h = _TILE_SIZE;
    _borders[HORIZONTAL_BOTTOM].w = _width;
    _borders[HORIZONTAL_BOTTOM].x = 0;
    _borders[HORIZONTAL_BOTTOM].y = _height - _TILE_SIZE;
    _borders[VERTICAL_RIGHT].h = _height;
    _borders[VERTICAL_RIGHT].w = _TILE_SIZE;
    _borders[VERTICAL_RIGHT].x = 0;
    _borders[VERTICAL_RIGHT].y = 0;
    _borders[VERTICAL_LEFT].h = _height;
    _borders[VERTICAL_LEFT].w = _TILE_SIZE;
    _borders[VERTICAL_LEFT].x = _width - _TILE_SIZE;
    _borders[VERTICAL_LEFT].y = 0;
}

void SDLGraphicLib::clearBoard() const
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);
}

void SDLGraphicLib::drawPlayer(const Snake_t &player)
{
    // Clear screen and render border walls
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 0);
    for (auto &border : _borders)
        SDL_RenderFillRect(_renderer, &border);

    // Render Player
    if (player.idx == 0)
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 0);
    else if (player.idx == 1)
        SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 0);
    for (auto &point : player.body)
    {
        _rect.x = point.x * _TILE_SIZE;
        _rect.y = point.y * _TILE_SIZE;
        SDL_RenderFillRect(_renderer, &_rect);
        if (point == player.body.front())
        {
            if (player.idx == 0)
                SDL_SetRenderDrawColor(_renderer, 175, 0, 0, 0);
            else
                SDL_SetRenderDrawColor(_renderer, 175, 175, 0, 0);
        }
    }
}

void SDLGraphicLib::drawFood(const point_t &point)
{
    SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 0);
    _rect.x = point.x * _TILE_SIZE;
    _rect.y = point.y * _TILE_SIZE;
    SDL_RenderFillRect(_renderer, &_rect);
    SDL_RenderPresent(_renderer);
}

void SDLGraphicLib::drawScores(int score, int highScore)
{
    (void)score;
    (void)highScore;
}

void SDLGraphicLib::registerPlayerInput() noexcept
{
    while (SDL_PollEvent(&_event) > 0)
    {
        switch (_event.type)
        {
        case SDL_KEYDOWN:
            switch (_event.key.keysym.scancode)
            {
            case SDL_SCANCODE_1:
                _arrayPlayerInput[0] = INPUT_SWAP_LIBNCURSES;
                break;
            case SDL_SCANCODE_3:
                _arrayPlayerInput[0] = INPUT_SWAP_LIBRAYLIB;
                break;
            case SDL_SCANCODE_ESCAPE:
                _arrayPlayerInput[0] = INPUT_QUIT;
                break;
            case SDL_SCANCODE_W:
                _arrayPlayerInput[0] = INPUT_UP;
                break;
            case SDL_SCANCODE_S:
                _arrayPlayerInput[0] = INPUT_DOWN;
                break;
            case SDL_SCANCODE_A:
                _arrayPlayerInput[0] = INPUT_LEFT;
                break;
            case SDL_SCANCODE_D:
                _arrayPlayerInput[0] = INPUT_RIGHT;
                break;
            case SDL_SCANCODE_UP:
                _arrayPlayerInput[1] = INPUT_UP;
                break;
            case SDL_SCANCODE_DOWN:
                _arrayPlayerInput[1] = INPUT_DOWN;
                break;
            case SDL_SCANCODE_LEFT:
                _arrayPlayerInput[1] = INPUT_LEFT;
                break;
            case SDL_SCANCODE_RIGHT:
                _arrayPlayerInput[1] = INPUT_RIGHT;
                break;
            case SDL_SCANCODE_M:
                _arrayPlayerInput[0] = INPUT_MUTE;
                break;
#ifdef DEBUG
            case SDL_SCANCODE_2:
                _arrayPlayerInput[0] = INPUT_SWAP_LIBSDL;
                break;
            case SDL_SCANCODE_4:
                _arrayPlayerInput[0] = INPUT_SWAP_LIBDEBUG;
                break;
#endif
            default:
                // Unutilized key
                break;
            }
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
    _arrayPlayerInput[0] = INPUT_DEFAULT;
    _arrayPlayerInput[1] = INPUT_DEFAULT;
}

SDLGraphicLib::~SDLGraphicLib()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

extern "C"
{
    SDLGraphicLib *makeGraphicLib(int width, int height)
    {
        return new SDLGraphicLib(width, height);
    }

    void destroyGraphicLib(SDLGraphicLib *gLib)
    {
        delete gLib;
    }
}