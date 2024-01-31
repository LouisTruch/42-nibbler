#include "../../inc/Menu/Menu.hpp"
#include "../../inc/Log/Logger.hpp"
#include <curses.h>

Menu::Menu() : _highlight(0)
{
    initscr();
    noecho();
    cbreak();

    _windowMenu = newwin(MENU_HEIGHT, MENU_WIDTH, 0, 0);
    box(_windowMenu, 0, 0);
    wrefresh(_windowMenu);

    keypad(_windowMenu, true);
    MenuCategory::VectorMenuItem vecGameMode;
    vecGameMode.push_back(std::make_unique<MenuItem>(MenuItem::GAME_MODE_STR[MenuItem::CHANGING_SPEED], false));
    vecGameMode.push_back(std::make_unique<MenuItem>(MenuItem::GAME_MODE_STR[MenuItem::MOVING_FOOD], false));
    vecGameMode.push_back(std::make_unique<MenuItem>(MenuItem::GAME_MODE_STR[MenuItem::HUNGER], false));
    _vecMenuCategory.push_back(std::make_unique<MenuCategory>("GAME MODE", true, std::move(vecGameMode)));

    MenuCategory::VectorMenuItem vecMultiplayer;
    vecMultiplayer.push_back(std::make_unique<MenuItem>(MenuItem::GAME_MODE_STR[MenuItem::SINGLE_PLAYER], false));
    vecMultiplayer.push_back(std::make_unique<MenuItem>(MenuItem::GAME_MODE_STR[MenuItem::MULTI_LOCAL], false));
    vecMultiplayer.push_back(std::make_unique<MenuItem>(MenuItem::GAME_MODE_STR[MenuItem::MULTI_NETWORK], false));
    _vecMenuCategory.push_back(std::make_unique<MenuCategory>("MULTIPLAYER", false, std::move(vecMultiplayer)));
}

Menu::~Menu()
{
    delwin(_windowMenu);
    endwin();
}

#include <stdexcept> // std::runtime_error
void Menu::printMenu(board_size_t boardSize)
{
    // Print box Title
    while (1)
    {
        wattron(_windowMenu, A_BOLD);
        wattron(_windowMenu, A_UNDERLINE);
        mvwprintw(_windowMenu, 0, MENU_WIDTH / 2 - 4, "NIBBLER");
        wattroff(_windowMenu, A_UNDERLINE);
        wattroff(_windowMenu, A_BOLD);
        mvwprintw(_windowMenu, 0, MENU_WIDTH / 2 + 5, "width:%i height:%i", boardSize.x, boardSize.y);

        // Print box content
        for (int y = 0; auto &category : _vecMenuCategory)
            category->printCategory(y++, _windowMenu, _highlight);

        if (_highlight == MenuItem::NB_ITEM)
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
            if (_highlight != MenuItem::NB_ITEM)
                _highlight++;
            break;
        case KEY_LEFT:
            if (_highlight != 0)
                _highlight--;
            break;
        case KEY_DOWN:
            if (_highlight <= 2)
                _highlight += 3;
            else if (_highlight <= 5)
                _highlight = MenuItem::NB_ITEM;
            break;
        case KEY_UP:
            if (_highlight >= 3)
                _highlight -= 3;
            break;
        case 32: // SpaceBar
            if (handleHighlighting())
                return;
            break;
        case 10: // Enter
            if (checkSingleChoiceCategory())
                return;
            break;
        case 27: // Escap
            throw std::runtime_error("Throw in Menu.cpp: printMenu(): Quit");
        default:
            break;
        }
    }
}

int Menu::handleHighlighting()
{
    if (_highlight == MenuItem::NB_ITEM)
    {
        if (!checkSingleChoiceCategory())
            return 0;
        return 1;
    }
    int categoryIdx = 0, itemIdx;
    findItemCategoryIdx(categoryIdx, itemIdx);
    _vecMenuCategory[categoryIdx]->selectItem(itemIdx);
    return 0;
}

// Check that one option is selected in single choice categories
bool Menu::checkSingleChoiceCategory() const
{
    for (auto &&category : _vecMenuCategory)
    {
        if (!category->getMultiChoice() && !category->getNbItemsSelected())
        {
            mvwprintw(_windowMenu, MENU_HEIGHT - 4, 1, "At least one option must be selected in MULTIPLAYER !");
            return false;
        }
    }
    return true;
}

void Menu::findItemCategoryIdx(int &categoryIdx, int &itemIdx)
{
    for (auto &&category : _vecMenuCategory)
    {
        itemIdx = 0;
        for (auto &&item : category->getVecMenuItem())
        {
            if (item->getItemIdx() == _highlight)
                return;
            itemIdx++;
        }
        categoryIdx++;
    }
}

#include <cmath> // std::pow
int_GameConfig_t Menu::exportGameConfig()
{
    int_GameConfig_t config = 0;

    for (auto &&category : _vecMenuCategory)
    {
        for (auto &&item : category->getVecMenuItem())
        {
            if (item->getSelected())
            {
                switch (item->getItemIdx())
                {
                case MenuItem::CHANGING_SPEED:
                    config |= (int)std::pow(2, item->getItemIdx());
                    break;
                case MenuItem::MOVING_FOOD:
                    config |= (int)std::pow(2, item->getItemIdx());
                    break;
                case MenuItem::HUNGER:
                    config |= (int)std::pow(2, item->getItemIdx());
                    break;
                case MenuItem::SINGLE_PLAYER:
                    config |= (int)std::pow(2, item->getItemIdx());
                    break;
                case MenuItem::MULTI_LOCAL:
                    config |= (int)std::pow(2, item->getItemIdx());
                    break;
                case MenuItem::MULTI_NETWORK:
                    config |= (int)std::pow(2, item->getItemIdx());
                    break;
                }
            }
        }
    }
    LOG_DEBUG("Game config: " << config);
    delwin(_windowMenu);
    endwin();
    return config;
}