#include "../../inc/Menu/Menu.hpp"
#include <cmath>
#include <curses.h>
#include <iostream>
#include <memory>

Menu::Menu()
{
    initscr();
    noecho();
    cbreak();

    _windowMenu = newwin(MENU_HEIGHT, MENU_WIDTH, 0, 0);
    box(_windowMenu, 0, 0);
    refresh();
    wrefresh(_windowMenu);

    keypad(_windowMenu, true);
    // std::make_unique(MenuCategory("GAME MODE", {MenuItem("OPTION", false, _windowMenu)}));
    MenuCategory::VectorMenuItem vecGameMode;
    vecGameMode.push_back(std::make_unique<MenuItem>("OPTION", false));
    vecGameMode.push_back(std::make_unique<MenuItem>("PLACEHOLDER", false));
    vecGameMode.push_back(std::make_unique<MenuItem>("FF", false));
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
    _highlight = 0;

    printMenu();
}

Menu::~Menu()
{
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
        case KEY_DOWN:
            if (_highlight == 8)
                break;
            _highlight++;
            break;
        case KEY_UP:
            if (_highlight == 0)
                break;
            _highlight--;
            break;
        case 32: // SpaceBar
            _vecMenuCategory[_highlight / _vecMenuCategory.size()]->selectItem(_highlight % _vecMenuCategory.size());
            break;
        case 10: // Enter
            break;
        case 27: // Escap
            return;
        default:
            std::cout << key << std::endl;

            break;
        }
    }
}

// for (int i = 0; i < NB_CATEGORY; i++)
// {

//     mvwprintw(_windowMenu, i * 2 + 1, 1, _menuChoice[i].data());
//     if (i == 0)
//     {
//         int line_idx = 0;
//         for (int j = 0; j < NB_GAMEMODE; j++)
//         {
//             mvwprintw(_windowMenu, i * 2 + 1 + 1, line_idx + PADDING_CHOICES, _gameModeChoice[j].data());
//             line_idx += _gameModeChoice[j].size() + PADDING_CHOICES;
//         }
//     }
//     else if (i == 1)
//     {
//         int line_idx = 0;
//         for (int j = 0; j < 2; j++)
//         {
//             mvwprintw(_windowMenu, i * 2 + 1 + 1, line_idx + PADDING_CHOICES, _soundChoice[j].data());
//             line_idx += _soundChoice[j].size() + PADDING_CHOICES;
//         }
//     }
//     else if (i == 2)
//     {
//         int line_idx = 0;
//         for (int j = 0; j < NB_GAMEMODE; j++)
//         {
//             mvwprintw(_windowMenu, i * 2 + 1 + 1, line_idx + PADDING_CHOICES, _multiplayerChoice[j].data());
//             line_idx += _multiplayerChoice[j].size() + PADDING_CHOICES;
//         }
//     }
// }
// mvwprintw(_windowMenu, MENU_HEIGHT - 2, MENU_WIDTH / 2 - 3, "START");
// wattroff(_windowMenu, A_BOLD);
// wrefresh(_windowMenu);