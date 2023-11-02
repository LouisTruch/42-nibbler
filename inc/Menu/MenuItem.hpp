#pragma once

#include <curses.h>
#include <string_view>

class MenuItem
{
  public:
    inline static int NB_ITEM = 0;
    typedef enum
    {
        CHANGING_SPEED = 0,
        DISAPPEARING_FOOD,
        HUNGER,
        SCORE,
        MULTI_OFF,
        MULTI_LOCAL,
        MULTI_NETWORK,
        SOUND,
    } game_mode_idx_e;
    static constexpr std::string_view GAME_MODE_STR[8] = {
        "CHANGING SPEED", "DISAPPEARING FOOD", "HUNGER", "SCORE", "MULTI OFF", "MULTI LOCAL", "MULTI NETWORK", "SOUND"};

  public:
    MenuItem(std::string_view, bool);
    ~MenuItem();
    std::string_view getItemName();
    void setSelected();
    bool getSelected() const;
    int getItemIdx() const;
    void print(int, WINDOW *, int, int);

  private:
    MenuItem() = delete;
    MenuItem(const MenuItem &) = delete;
    MenuItem &operator=(const MenuItem &) = delete;

  private:
    game_mode_idx_e _itemIdx;
    std::string_view _itemName;
    bool _isSelected;
};