#pragma once
#include "../types.hpp"
#include "MenuCategory.hpp"
#include "MenuItem.hpp"
#include <cstdint>
#include <curses.h>
#include <map>
#include <memory>
#include <ncurses.h>
#include <string_view>
#include <vector>

class Menu
{
  public:
    typedef std::vector<std::unique_ptr<MenuCategory>> VectorMenuCategory;

  public:
    Menu();
    ~Menu();
    void printMenu(board_size_t);
    int handleHighlighting();
    void findItemCategoryIdx(int &, int &);
    int_gameConfig_t exportGameConfig();
    bool checkSingleChoiceCategory() const;

  private:
    Menu(const Menu &) = delete;
    Menu &operator=(const Menu &) = delete;

  private:
    static constexpr int MENU_HEIGHT = 12;
    static constexpr int MENU_WIDTH = 70;
    int _highlight;
    WINDOW *_windowMenu;
    VectorMenuCategory _vecMenuCategory;
};