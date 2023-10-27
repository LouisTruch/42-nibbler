#include "../inc/SfmlGraphicLib.hpp"

#include <iostream>
SfmlGraphicLib::SfmlGraphicLib(int width, int height)
{
    std::cout << "ici\n";
    // sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Nibbler");
    // _window.create(sf::VideoMode(220, 200), "tg");
    sf::VideoMode(200, 200);
}

void SfmlGraphicLib::drawPlayer(const body_t &body)
{
    (void)body;
}

void SfmlGraphicLib::drawFood(const point_t &point)
{
    (void)point;
}

void SfmlGraphicLib::registerPlayerInput()
{
}

player_input_t SfmlGraphicLib::getPlayerInput() const
{
    return _playerInput;
}

void SfmlGraphicLib::resetPlayerInput()
{
    _playerInput = DEFAULT;
}

SfmlGraphicLib::SfmlGraphicLib()
{
}

SfmlGraphicLib::~SfmlGraphicLib()
{
}

SfmlGraphicLib::SfmlGraphicLib(const SfmlGraphicLib &other)
{
    *this = other;
}

SfmlGraphicLib &SfmlGraphicLib::operator=(const SfmlGraphicLib &other)
{
    if (&other == this)
        return *this;
    return *this;
}

std::unique_ptr<SfmlGraphicLib> makeGraphicLib(int width, int height)
{
    return std::make_unique<SfmlGraphicLib>(width, height);
}

void destroyGraphicLib(std::unique_ptr<SfmlGraphicLib> gLib)
{
    gLib.reset();
}