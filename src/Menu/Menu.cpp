#include "../../inc/Menu/Menu.hpp"
#include <curses.h>
#include <iostream>

Menu::Menu() : _highlight(0)
{
    initscr();
    noecho();
    cbreak();

    _windowMenu = newwin(MENU_HEIGHT, MENU_WIDTH, 0, 0);
    box(_windowMenu, 0, 0);
    // refresh();
    wrefresh(_windowMenu);

    keypad(_windowMenu, true);
    MenuCategory::VectorMenuItem vecGameMode;
    vecGameMode.push_back(std::make_unique<MenuItem>("CHANGING_SPEED", false));
    vecGameMode.push_back(std::make_unique<MenuItem>("DISAPPEARING_FOOD", false));
    _vecMenuCategory.push_back(std::make_unique<MenuCategory>("GAME MODE", true, std::move(vecGameMode)));

    MenuCategory::VectorMenuItem vecMultiplayer;
    vecMultiplayer.push_back(std::make_unique<MenuItem>("OFF", true));
    vecMultiplayer.push_back(std::make_unique<MenuItem>("LOCAL", false));
    vecMultiplayer.push_back(std::make_unique<MenuItem>("NETWORK", false));
    _vecMenuCategory.push_back(std::make_unique<MenuCategory>("MULTIPLAYER", false, std::move(vecMultiplayer)));

    MenuCategory::VectorMenuItem vecSound;
    vecSound.push_back(std::make_unique<MenuItem>("OFF", true));
    vecSound.push_back(std::make_unique<MenuItem>("ON", false));
    _vecMenuCategory.push_back(std::make_unique<MenuCategory>("SOUND", false, std::move(vecSound)));

    // MenuCategory::VectorMenuItem vecPseudo;
    // vecPseudo.push_back(std::make_unique<MenuItem>("__", false));
    // _vecMenuCategory.push_back(std::make_unique<MenuCategory>("PSEUDO", false, std::move(vecPseudo)));

    printMenu();
}

Menu::~Menu()
{
    delwin(_windowMenu);
    endwin();
}

void Menu::printMenu()
{
    // Print box Title
    while (1)
    {
        wattron(_windowMenu, A_BOLD);
        wattron(_windowMenu, A_UNDERLINE);
        mvwprintw(_windowMenu, 0, MENU_WIDTH / 2 - 4, "NIBBLER");
        wattroff(_windowMenu, A_UNDERLINE);
        wattroff(_windowMenu, A_BOLD);

        // Print box content
        for (int y = 0; auto &category : _vecMenuCategory)
            category->printCategory(y++, _windowMenu, _highlight);

        if (_highlight == 8)
            wattron(_windowMenu, A_REVERSE);
        wattron(_windowMenu, A_BOLD);
        mvwprintw(_windowMenu, MENU_HEIGHT - 2, MENU_WIDTH / 2 - 3, "START");
        wattroff(_windowMenu, A_BOLD);
        wattroff(_windowMenu, A_REVERSE);

        wrefresh(_windowMenu);
        int key = wgetch(_windowMenu);
        switch (key)
        {
        case KEY_RIGHT:
            if (_highlight != 8)
                _highlight++;
            break;
        case KEY_LEFT:
            if (_highlight != 0)
                _highlight--;
            break;
        case KEY_DOWN:
            if (_highlight <= 5)
                _highlight += 3;
            break;
        case KEY_UP:
            if (_highlight >= 3)
                _highlight -= 3;
            break;
        case 32: // SpaceBar
            _vecMenuCategory[_highlight / _vecMenuCategory.size()]->selectItem(_highlight % _vecMenuCategory.size());
            break;
        case 10: // Enter
                 /// Export config
            break;
        case 27: // Escap
            return;
        default:
            // std::cout << key << std::endl;
            break;
        }
    }
}

int_gameConfig_t Menu::exportGameConfig()
{
    int_gameConfig_t config = 0;
    for (int i = 0; auto &&category : _vecMenuCategory)
    {
        if (category->getMultiChoice())
        {
            for (auto &&item : category->getVecMenuItem())
            {
                if (item->getSelected())
                {
                    switch (item->getItemIdx())
                    {
                    case SPEED:
                        config |= SPEED_BIT;
                        break;
                    case DISAPPEARING_FOOD:
                        config |= DISAPPEARING_FOOD_BIT;
                        break;
                    }
                }
                i++;
            }
        }
    }
    return config;
}