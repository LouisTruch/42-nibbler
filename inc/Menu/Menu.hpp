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
#include <tuple>
#include <vector>

class Menu
{
  public:
    typedef std::vector<std::unique_ptr<MenuCategory>> VectorMenuCategory;


    // struct gameConfig
    // {
    //     bool _changingSpeed = false;
    //     bool _sound = false;
    //     int _multiplayer = 0;
    // };

  public:
    Menu();
    ~Menu();
    void printMenu();
    int_gameConfig_t exportGameConfig();

  private:
    Menu(const Menu &) = delete;
    Menu &operator=(const Menu &) = delete;

  private:
    static constexpr int MENU_HEIGHT = 12;
    static constexpr int MENU_WIDTH = 60;
    int _highlight;
    WINDOW *_windowMenu;
    VectorMenuCategory _vecMenuCategory;
};