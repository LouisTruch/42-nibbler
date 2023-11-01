#pragma once
#include "MenuCategory.hpp"
#include "MenuItem.hpp"
#include <curses.h>
#include <map>
#include <memory>
#include <ncurses.h>
#include <string_view>
#include <vector>

class Menu
{
  public:
    Menu();
    ~Menu();
    void printMenu();

  private:
    Menu(const Menu &) = delete;
    Menu &operator=(const Menu &) = delete;

  private:
    int _highlight;
    WINDOW *_windowMenu;
    static constexpr int MENU_HEIGHT = 12;
    static constexpr int MENU_WIDTH = 60;

    typedef std::vector<std::unique_ptr<MenuCategory>> VectorMenuCategory;
    VectorMenuCategory _vecMenuCategory;
};