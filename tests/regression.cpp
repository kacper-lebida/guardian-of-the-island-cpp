#include "Character.h"
#include "SaveSystem.h"
#include "Game.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <climits>

void require(bool condition, const char* label) {
    if (!condition) throw std::runtime_error(label);
}

void writeSave(const std::string& text) {
    std::ofstream("save.txt") << text;
}

int main() {
    try {
        Character source("Test Hero", 90, 15);
        source.inventory.addItem(std::make_unique<WeaponItem>("Iron Sword", 60, 5));
        source.inventory.addItem(std::make_unique<HealingItem>("Small Potion", 20, 20));
        source.inventory.addItem(std::make_unique<ArmorItem>("Old Helmet", 30, 8));
        require(SaveSystem::save(source, 4, 10), "save success");
        Character loaded("Unchanged", 33, 7);
        int survived = 0, goal = 1;
        require(SaveSystem::load(loaded, survived, goal), "load success");
        require(loaded.name == "Test Hero" && loaded.hp == 90 && survived == 4 && goal == 10, "round trip statistics");
        require(loaded.inventory.getItemCount() == 3, "round trip item count");
        loaded.inventory.useItem(1, loaded);
        loaded.inventory.useItem(1, loaded);
        loaded.inventory.useItem(1, loaded);
        require(loaded.attack == 20 && loaded.hp == 118 && loaded.inventory.isEmpty(), "restored item behavior");
        bool rejected = false;
        try { loaded.inventory.useItem(0, loaded); } catch (const std::out_of_range&) { rejected = true; }
        require(rejected, "invalid inventory index");

        const std::string malformed[] = {
            "", "Hero\n100\n", "Hero\n100\n15\n0\n10\n1\nWEAPON\nSword\n",
            "Hero\n100\n15\n0\n10\n1\nUNKNOWN\nSword\n1\n2\n",
            "Hero\n100\n15\n0\n10\n-1\n", "Hero\n100\n15\n11\n10\n0\n",
            "Hero\n100\n15\n0\n10\n0\ntrailing", "Hero\n100\n15\n0\n10\n10001\n"
        };
        for (const auto& data : malformed) {
            writeSave(data);
            require(!SaveSystem::load(loaded, survived, goal), "reject corrupt save");
            require(loaded.name == "Test Hero" && loaded.hp == 118 && loaded.attack == 20 && survived == 4,
                    "corrupt save must leave character and progress untouched");
        }
        std::filesystem::remove("save.txt");
        require(!SaveSystem::load(loaded, survived, goal), "missing save");
        std::filesystem::create_directory("save.txt");
        require(!SaveSystem::save(source, 0, 10), "unwritable save reports failure");
        std::filesystem::remove("save.txt");
        loaded.heal(INT_MAX);
        loaded.increaseAttack(INT_MAX);
        require(loaded.hp == 1000000 && loaded.attack == 1000000, "bounded statistics");

        Game game;
        game.restartGame();
        game.restartGame();
        game.save();
        require(SaveSystem::load(loaded, survived, goal), "restart save");
        require(loaded.inventory.getItemCount() == 3 && survived == 0 && goal == 10 && loaded.hp == 100,
                "restart restores one starter inventory and default progress");
        writeSave("Hero\n100000\n92\n9\n10\n0\n");
        Game finale;
        finale.startCombat(Character("Regression enemy", 1, 0));
        std::string attacks;
        for (int i = 0; i < 100; ++i) attacks += "1\n";
        std::istringstream input(attacks);
        std::ostringstream output;
        auto* oldIn = std::cin.rdbuf(input.rdbuf());
        auto* oldOut = std::cout.rdbuf(output.rdbuf());
        finale.combatLoop();
        finale.run();
        std::cin.rdbuf(oldIn);
        std::cout.rdbuf(oldOut);
        std::cin.clear();
        require(output.str().find("YOU WON") != std::string::npos,
                "winning the final regular fight must transition to the boss");
        std::filesystem::remove("save.txt");
        std::cout << "Core regression tests passed.\n";
    } catch (const std::exception& e) {
        std::cerr << "FAILED: " << e.what() << "\n";
        return 1;
    }
}
