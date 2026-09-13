#pragma once
#include <vector>
#include <memory>
#include <fstream>
#include "Item.h"

class Character;

// Owns its items via unique_ptr (polymorphic storage, auto-cleanup).
class Inventory {
private:
    std::vector<std::unique_ptr<Item>> items;

public:
    void addItem(std::unique_ptr<Item> item);
    void showItems() const;
    void useItem(int index, Character& player);
    bool isEmpty() const;
    int getItemCount() const;

    void clear();
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
};
