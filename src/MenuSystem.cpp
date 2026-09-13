#include "MenuSystem.h"
#include "Game.h"
#include <iostream>
#include <cctype>
#include <string>

void MenuSystem::showMenu() {
    std::cout << "\n=== MENU ===\n";
    std::cout << "[E] Explore\n";
    std::cout << "[I] Show Inventory\n";
    std::cout << "[U] Use Item\n";
    std::cout << "[S] Save\n";
    std::cout << "[Q] Quit\n";
    std::cout << "Choice: ";
}


char MenuSystem::getInput() {
    std::string line;
    if (!std::getline(std::cin, line)) return 'q';
    if (line.size() != 1) return '\0';
    return static_cast<char>(std::tolower(static_cast<unsigned char>(line[0])));
}

void MenuSystem::handleInput(Game& game, char input) {
    switch (input) {
        case 'e': game.explore(); break;
        case 'i': game.showInventory(); break;
        case 's': game.save(); break;
        case 'q': game.quit(); break;
        case 'u': game.useInventoryItem();break;
        default: std::cout << "Invalid option\n";
    }
}
