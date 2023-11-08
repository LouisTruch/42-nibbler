#pragma once

#include "MenuItem.hpp"
#include <curses.h>
#include <memory>
#include <string_view>
#include <vector>

class MenuCategory
{
  public:
    typedef std::vector<std::unique_ptr<MenuItem>> VectorMenuItem;

  public:
    MenuCategory(std::string_view, bool, std::vector<std::unique_ptr<MenuItem>> &&);
    ~MenuCategory();
    bool operator<(const MenuCategory &) const;
    void printCategory(int, WINDOW *, int);
    void selectItem(int);
    const VectorMenuItem &getVecMenuItem() const;
    bool getMultiChoice() const;
    std::string_view getCategoryName() const;
    int getNbItemsSelected() const;

  private:
    MenuCategory() = delete;
    MenuCategory(const MenuCategory &) = delete;
    MenuCategory &operator=(const MenuCategory &) = delete;

  private:
    std::string_view _categoryName;
    VectorMenuItem _vecMenuItem;
    bool _isMultiChoice;
    int _nbSelected;
    static const int PADDING_CATEGORY = 1;
    static const int PADDING_ITEMS = 3;
    static const int PADDING_BETWEEN_ITEMS = 5;
};