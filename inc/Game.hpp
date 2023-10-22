#pragma once
#include <vector>
#include <memory>
#include "GraphicHandler.hpp"
#include "LibHandler.hpp"

class Game
{
public:
    Game(int, int);
    ~Game();
    Game(const Game &);
    Game &operator=(const Game &);

    void loop();

public:
    std::unique_ptr<IGraphicLib> _gHandler;
    std::unique_ptr<LibHandler> _libHandler;

private:
    Game();

private:
    typedef u_int8_t tile_t;
    typedef std::vector<tile_t> line_t;
    typedef std::vector<std::vector<tile_t>> map_t;
    map_t _map;
};