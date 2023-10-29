#pragma once

#include "../raylib/src/raylib.h"
#include <map>
#include <string>
#include <string_view>

class RaylibTexture
{
  public:
    typedef enum
    {
        BODY_BOTTOMLEFT,
        BODY_BOTTOMRIGHT,
        BODY_HORIZONTAL,
        BODY_TOPLEFT,
        BODY_TOPRIGHT,
        BODY_VERTICAL,
        FOOD,
        HEAD_DOWN,
        HEAD_LEFT,
        HEAD_RIGHT,
        HEAD_UP,
        TAIL_DOWN,
        TAIL_LEFT,
        TAIL_RIGHT,
        TAIL_UP,
        TEXTURE_NB_ITEMS,
        BACKGROUND,
    } texture_name_e;

  public:
    RaylibTexture(texture_name_e);
    RaylibTexture(int, int, int, Color, Color);
    ~RaylibTexture();
    Texture2D getTexture();
    RaylibTexture(const RaylibTexture &);
    RaylibTexture &operator=(const RaylibTexture &);

  private:
    RaylibTexture() = delete;

  private:
    typedef std::map<texture_name_e, std::string> TextureMap;
    inline static TextureMap _textureMap = {
        {BODY_BOTTOMLEFT, "body_bottomleft.png"},
        {BODY_BOTTOMRIGHT, "body_bottomright.png"},
        {BODY_HORIZONTAL, "body_horizontal.png"},
        {BODY_TOPLEFT, "body_topleft.png"},
        {BODY_TOPRIGHT, "body_topright.png"},
        {BODY_VERTICAL, "body_vertical.png"},
        {FOOD, "food.png"},
        {HEAD_DOWN, "head_down.png"},
        {HEAD_LEFT, "head_left.png"},
        {HEAD_RIGHT, "head_right.png"},
        {HEAD_UP, "head_up.png"},
        {TAIL_DOWN, "tail_down.png"},
        {TAIL_LEFT, "tail_left.png"},
        {TAIL_RIGHT, "tail_right.png"},
        {TAIL_UP, "tail_up.png"},
    };
    inline static std::string _textureFilePath = "./libs/raylib/assets/";
    texture_name_e _textureName;
    Texture2D _texture;
};