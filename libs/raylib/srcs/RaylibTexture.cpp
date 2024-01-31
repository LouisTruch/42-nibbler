#include "../inc/RaylibTexture.hpp"

RaylibTexture::RaylibTexture(texture_name_e textureName) : _textureName(textureName)
{
    Image img = LoadImage((_textureFilePath + _textureMap.find(_textureName)->second).c_str());
    _texture = LoadTextureFromImage(img);
    UnloadImage(img);
}

// Create checkerboard texture with color1 & color2
RaylibTexture::RaylibTexture(int width, int height, int tileSize, Color color1, Color color2)
{
    Image img = GenImageColor(width - (tileSize * 2), height - (tileSize * 2), color1);
    for (int i = 0; i < ((width - 2) / tileSize); i++)
        if (i % 2)
            for (int j = 0; j < ((height - 2) / tileSize); j++)
                if (j % 2)
                {
                    ImageDrawRectangle(&img, i * tileSize, j * tileSize, tileSize, tileSize, color2);
                    ImageDrawRectangle(&img, i * tileSize - tileSize, j * tileSize - tileSize, tileSize, tileSize,
                                       color2);
                }
    _texture = LoadTextureFromImage(img);
    UnloadImage(img);
}

RaylibTexture::~RaylibTexture()
{
    UnloadTexture(_texture);
}

RaylibTexture::RaylibTexture(const RaylibTexture &other)
{
    *this = other;
}

RaylibTexture &RaylibTexture::operator=(const RaylibTexture &other)
{
    if (&other == this)
        return *this;
    _textureName = other._textureName;
    Image img = LoadImage((_textureFilePath + _textureMap.find(_textureName)->second).c_str());
    _texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return *this;
}

Texture2D RaylibTexture::getTexture()
{
    return _texture;
}
