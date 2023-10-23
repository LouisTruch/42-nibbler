#pragma once
#include <memory>
#include "GraphicHandler.hpp"
#include "LibHandler.hpp"
#include "IGraphicLib.hpp"

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
    LibHandler _libHandler;

private:
    Game();

private:
    IGraphicLib::map_t _map;
};