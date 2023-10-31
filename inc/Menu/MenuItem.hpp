#pragma once

#include <curses.h>
#include <string_view>

class MenuItem
{
  public:
    MenuItem(std::string_view, bool);
    ~MenuItem();
    std::string_view getItemName();
    void setSelected();
    bool getSelected();
    void print(int, WINDOW *, int, int);

  private:
    MenuItem() = delete;
    MenuItem(const MenuItem &) = delete;
    MenuItem &operator=(const MenuItem &) = delete;

  private:
    inline static int _itemNb = 0;
    int _itemIdx;
    std::string_view _itemName;
    bool _isSelected;
};