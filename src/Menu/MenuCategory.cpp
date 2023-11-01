#include "../../inc/Menu/MenuCategory.hpp"
#include <curses.h>
#include <iostream>

MenuCategory::MenuCategory(std::string_view categoryName, bool multiChoice,
                           std::vector<std::unique_ptr<MenuItem>> &&vecMenuItem)
    : _vecMenuItem(std::move(vecMenuItem)), _isMultiChoice(multiChoice), _nbSelected(0)
{
    for (auto &menuItem : _vecMenuItem)
    {
        if (menuItem->getSelected())
            _nbSelected++;
    }
    _categoryName = categoryName;
    // _nbCategory++;
}

MenuCategory::~MenuCategory()
{
}

bool MenuCategory::operator<(const MenuCategory &other) const
{
    (void)other;
    return false;
}

void MenuCategory::printCategory(int y, WINDOW *win, int highlight)
{
    wattron(win, A_BOLD);
    mvwprintw(win, y * 2 + 1, 1, _categoryName.data());
    wattroff(win, A_BOLD);
    int line_idx = 0;
    for (auto &menuItem : _vecMenuItem)
    {
        menuItem->print(y * 2 + 1 + 1, win, line_idx + PADDING_ITEMS, highlight);
        line_idx += menuItem->getItemName().size() + PADDING_BETWEEN_ITEMS;
    }
}

void MenuCategory::selectItem(int hightlight)
{
    if (hightlight >= _vecMenuItem.size())
        return;
    if ((!_vecMenuItem[hightlight]->getSelected()))
    {
        if (_isMultiChoice || (!_isMultiChoice && _nbSelected == 0))
        {
            _vecMenuItem[hightlight]->setSelected();
            _nbSelected++;
        }
    }
    else if (_vecMenuItem[hightlight]->getSelected())
    {
        _vecMenuItem[hightlight]->setSelected();
        _nbSelected--;
    }
}