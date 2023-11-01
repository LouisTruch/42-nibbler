#include "../../inc/Menu/MenuItem.hpp"
#include <curses.h>
#include <iostream>

MenuItem::MenuItem(std::string_view itemName, bool selected)
{
    _itemName = itemName;
    _isSelected = selected;
    _itemIdx = _itemNb++;
}

MenuItem::~MenuItem()
{
}

std::string_view MenuItem::getItemName()
{
    return _itemName;
}

void MenuItem::print(int y, WINDOW *win, int x, int highlight)
{
    if (highlight == _itemIdx)
        wattron(win, A_REVERSE);
    if (!_isSelected)
    {
        mvwprintw(win, y, x, "[ ]");
        mvwprintw(win, y, x + 3, _itemName.data());
    }
    if (_isSelected)
    {
        wattron(win, A_BOLD);
        mvwprintw(win, y, x, "[o]");
        mvwprintw(win, y, x + 3, _itemName.data());
        wattroff(win, A_BOLD);
    }
    wattroff(win, A_REVERSE);
}

void MenuItem::setSelected()
{
    _isSelected = _isSelected ? false : true;
}

bool MenuItem::getSelected() const
{
    return _isSelected;
}

int MenuItem::getItemIdx() const
{
    return _itemIdx;
}