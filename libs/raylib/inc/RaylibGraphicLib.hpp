#pragma once

#include "../../../inc/IGraphicLib.hpp"
#include "../raylib/src/raylib.h"
#include "RaylibTexture.hpp"
#include <memory>
#include <stddef.h>
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
    // void drawPlayer(const body_t &);
    virtual void drawPlayer(const Player &);

    void drawFood(const point_t &);

  private:
    RaylibGraphicLib() = delete;

  private:
    int _width;
    int _height;
    std::vector<RaylibTexture> _vecTexture;
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