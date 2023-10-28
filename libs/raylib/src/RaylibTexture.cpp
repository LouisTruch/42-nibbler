#include "../inc/RaylibTexture.hpp"

RaylibTexture::RaylibTexture(texture_name_e textureName) : _textureName(textureName)
{
    Image img = LoadImage((_textureFilePath + _textureMap.find(_textureName)->second).c_str());
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
    UnloadTexture(_texture);
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
