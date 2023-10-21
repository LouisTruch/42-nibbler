#pragma once
#include <vector>

class Game
{
public:
    Game(int, int);
    ~Game();
    Game(const Game &);
    Game &operator=(const Game &);

    // IGraphicHandler *GraphicHandler;

private:
    Game();

private:
    typedef u_int8_t tile_t;
    typedef std::vector<tile_t> line_t;
    typedef std::vector<std::vector<tile_t>> map_t;
    map_t _map;
};