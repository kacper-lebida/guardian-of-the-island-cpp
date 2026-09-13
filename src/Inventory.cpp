#include "Inventory.h"
#include "Character.h"
#include <iostream>
#include <stdexcept>
#include <limits>

void Inventory::addItem(std::unique_ptr<Item> item) {
    items.push_back(std::move(item));
}

void Inventory::showItems() const {
    if (items.empty()) {
        std::cout << "Inventory empty.\n";
        return;
    }

    std::cout << "\n=== INVENTORY ===\n";

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << i + 1 << ". " << items[i]->getDescription() << "\n";
    }
}
// index is 1-based (matches the numbering shown to the player).
// Throws std::out_of_range if the index is invalid.
void Inventory::useItem(int index, Character& player) {
    if (index < 1 || index > static_cast<int>(items.size())) {
        throw std::out_of_range("Invalid item index.");
    }

    items[index - 1]->use(player);
    items.erase(items.begin() + index - 1);
}

bool Inventory::isEmpty() const {
    return items.empty();
}

int Inventory::getItemCount() const {
    return static_cast<int>(items.size());
}

void Inventory::clear() {
    items.clear();
}

void Inventory::saveToFile(std::ofstream& file) const {
    file << items.size() << "\n";

    for (const auto& item : items) {
        file << item->getType() << "\n";
        file << item->getName() << "\n";
        file << item->getValue() << "\n";
        file << item->getPower() << "\n";
    }
}

void Inventory::loadFromFile(std::ifstream& file) {
    Inventory candidate;
    int count = 0;
    if (!(file >> count) || count < 0 || count > 10000)
        throw std::runtime_error("Invalid inventory count.");
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int i = 0; i < count; ++i) {
        std::string type, name;
        int value = 0, power = 0;
        std::getline(file, type);
        std::getline(file, name);
        if (!(file >> value >> power) || name.empty() || name.size() > 100 ||
            value < 0 || value > 1000000 || power < 0 || power > 1000000)
            throw std::runtime_error("Invalid item data.");
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (type == "WEAPON")
            candidate.addItem(std::make_unique<WeaponItem>(name, value, power));
        else if (type == "HEALING")
            candidate.addItem(std::make_unique<HealingItem>(name, value, power));
        else if (type == "ARMOR")
            candidate.addItem(std::make_unique<ArmorItem>(name, value, power));
        else throw std::runtime_error("Unknown item type.");
    }
    items = std::move(candidate.items);
}
