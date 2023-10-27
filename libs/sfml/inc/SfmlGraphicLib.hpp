#pragma once

#include "../../../inc/IGraphicLib.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stddef.h>

constexpr int SFML_TILE_SIZE = 5;

class SfmlGraphicLib : public IGraphicLib
{
  public:
    SfmlGraphicLib(int, int);
    ~SfmlGraphicLib();
    SfmlGraphicLib(const SfmlGraphicLib &);
    SfmlGraphicLib &operator=(const SfmlGraphicLib &);
    player_input_t getPlayerInput() const;
    void resetPlayerInput();
    void registerPlayerInput();
    void drawPlayer(const body_t &);
    void drawFood(const point_t &);

  private:
    SfmlGraphicLib();

  private:
    sf::RenderWindow *_window;
    sf::VideoMode *_video;
};

extern "C"
{
    std::unique_ptr<SfmlGraphicLib> makeGraphicLib(int width, int height);
    void destroyGraphicLib(std::unique_ptr<SfmlGraphicLib> gLib);
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