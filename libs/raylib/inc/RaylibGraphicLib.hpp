#pragma once

#include "../../../inc/IGraphicLib.hpp"
#include "../raylib/src/raylib.h"
#include "RaylibTexture.hpp"
#include <algorithm>
#include <deque>
#include <memory>
#include <string_view>
#include <vector>

class RaylibGraphicLib : public IGraphicLib
{
  public:
    RaylibGraphicLib(int, int);
    ~RaylibGraphicLib();
    RaylibGraphicLib(const RaylibGraphicLib &);
    RaylibGraphicLib &operator=(const RaylibGraphicLib &);
    player_input_t getPlayerInput() const;
    void resetPlayerInput();
    void registerPlayerInput();
    void drawPlayer(const Player &);
    void drawFood(const point_t &);

  private:
    RaylibGraphicLib() = delete;
    void drawHead(int, int, int);
    void drawTail(std::deque<point_t>::const_iterator &);
    void drawBody(std::deque<point_t>::const_iterator &);
    // DrawTexture() wrapper to avoid having to add TILE_SIZE to every call;
    void drawTextureTileSize(Texture2D, int, int, Color);

  private:
    int _width;
    int _height;
    std::vector<RaylibTexture> _vecTexture;
    std::unique_ptr<RaylibTexture> _background;
    const Color _boardColor1 = {174, 220, 79, 255};
    const Color _boardColor2 = {146, 206, 63, 255};
    static constexpr int TILE_SIZE = 40;
};

extern "C"
{
    std::unique_ptr<RaylibGraphicLib> makeGraphicLib(int width, int height);
    void destroyGraphicLib(std::unique_ptr<RaylibGraphicLib> gLib);
}

// int main()
// {
// sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//     sf::CircleShape shape(100.f);
//     shape.setFillColor(sf::Color::Green);
//     sf::RectangleShape rec(sf::Vector2f(50, 50));
//     rec.setFillColor(sf::Color::Red);
//     sf::Texture texture;
//     if (!texture.loadFromFile("body_topleft.png"))
//     {
//         return 1;
//     }
//     rec.setTexture(&texture);
//     rec.setFillColor(sf::Color(0, 255, 0));

//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         window.clear();
//         // window.draw(shape);
//         window.draw(rec);
//         window.display();
//     }

//     return 0;
// }