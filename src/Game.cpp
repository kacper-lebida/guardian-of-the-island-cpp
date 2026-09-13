#include "Game.h"
#include <memory>
#include "Character.h"
#include "Item.h"
#include "SaveSystem.h"
#include <iostream>
#include <cstdlib>
#include "Input.h"
#include <algorithm>
#include <ctime>

Game::Game()
    : player("Hero", 100, 15),
      enemy("None", 0, 0),
      state(GameState::EXPLORATION),
      locationsSurvived(0),
      locationsToWin(10),
      enemiesLeftInLocation(0),
      finalBossStarted(false)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    
    locations = {
        "Dark Forest",
        "Abandoned Mine",
        "Old Road",
        "Silent Cave",
        "Broken Bridge",
        "Ruined Village",
        "Foggy Swamp",
        "Hunter Camp",
        "Ancient Tower",
        "Frozen Path",
        "Burned Farm",
        "Hidden Valley",
        "Stone Gate",
        "Wild River",
        "Lost Cemetery",
        "Empty Castle",
        "Mountain Pass",
        "Deep Woods",
        "Old Chapel",
        "Bandit Trail"
    };
    
    
    player.inventory.addItem(std::make_unique<WeaponItem>("Rusty Sword", 20, 2));
    player.inventory.addItem(std::make_unique<HealingItem>("Small Potion", 15, 20));
    player.inventory.addItem(std::make_unique<ArmorItem>("Wooden Shield", 25, 10));
    
    if (SaveSystem::load(player, locationsSurvived, locationsToWin)) {
        std::cout << "Save loaded.\n";
        checkWinCondition();
    }
    
    
}// Rolls a random item (weapon / armor / potion) and add it to the inventory.
void Game::addRandomLoot() {
    int lootType = rand() % 20;

    if (lootType == 0)
        player.inventory.addItem(std::make_unique<HealingItem>("Small Potion", 20, 20));
    else if (lootType == 1)
        player.inventory.addItem(std::make_unique<HealingItem>("Health Potion", 35, 35));
    else if (lootType == 2)
        player.inventory.addItem(std::make_unique<HealingItem>("Big Potion", 60, 60));
    else if (lootType == 3)
        player.inventory.addItem(std::make_unique<HealingItem>("Mega Potion", 100, 90));
    else if (lootType == 4)
        player.inventory.addItem(std::make_unique<WeaponItem>("Rusty Dagger", 20, 2));
    else if (lootType == 5)
        player.inventory.addItem(std::make_unique<WeaponItem>("Iron Sword", 60, 5));
    else if (lootType == 6)
        player.inventory.addItem(std::make_unique<WeaponItem>("Steel Axe", 80, 8));
    else if (lootType == 7)
        player.inventory.addItem(std::make_unique<WeaponItem>("Knight Sword", 120, 12));
    else if (lootType == 8)
        player.inventory.addItem(std::make_unique<WeaponItem>("Rare Blade", 180, 18));
    else if (lootType == 9)
        player.inventory.addItem(std::make_unique<WeaponItem>("Dragon Slayer", 250, 25));
    else if (lootType == 10)
        player.inventory.addItem(std::make_unique<ArmorItem>("Old Helmet", 30, 8));
    else if (lootType == 11)
        player.inventory.addItem(std::make_unique<ArmorItem>("Leather Armor", 60, 15));
    else if (lootType == 12)
        player.inventory.addItem(std::make_unique<ArmorItem>("Iron Armor", 100, 25));
    else if (lootType == 13)
        player.inventory.addItem(std::make_unique<ArmorItem>("Steel Armor", 150, 35));
    else if (lootType == 14)
        player.inventory.addItem(std::make_unique<ArmorItem>("Guardian Plate", 220, 50));
    else if (lootType == 15)
        player.inventory.addItem(std::make_unique<HealingItem>("Minor Potion", 15, 15));
    else if (lootType == 16)
        player.inventory.addItem(std::make_unique<WeaponItem>("Hunter Bow", 90, 10));
    else if (lootType == 17)
        player.inventory.addItem(std::make_unique<WeaponItem>("War Hammer", 140, 15));
    else if (lootType == 18)
        player.inventory.addItem(std::make_unique<ArmorItem>("Magic Cloak", 130, 30));
    else
        player.inventory.addItem(std::make_unique<HealingItem>("Ancient Elixir", 200, 120));

    std::cout << "Loot added to inventory.\n";
}

void Game::run() {
    while (state != GameState::EXIT) {

        if (state == GameState::EXPLORATION) {
            menu.showMenu();
            char input = menu.getInput();
            // Keep output visible for terminal history and redirected sessions.
            menu.handleInput(*this, input);
        }
        else if (state == GameState::COMBAT) {
            combatLoop();
        }
    }
}

void Game::explore() {
    int index = rand() % locations.size();

    std::cout << "\nLocation "
              << locationsSurvived + 1
              << "/"
              << locationsToWin
              << ": "
              << locations[index]
              << "\n";

    randomEvent();
}
void Game::randomEvent() { // Each explored location rolls one of 3 outcomes: nothing / loot / combat.
    int event = rand() % 3;

    if (event == 0) {
        std::cout << "You found nothing.\n";
        locationsSurvived++;
        checkWinCondition();
    }
    else if (event == 1) {
        std::cout << "You found abandoned supplies.\n";

        addRandomLoot();
        locationsSurvived++;
        state = GameState::EXPLORATION;
        checkWinCondition();
        return;
    }
    else {
        Character randomEnemy = createRandomEnemy();

        std::cout << randomEnemy.name << " attacks you!\n";
        enemiesLeftInLocation = 1 + rand() % 5;

        std::cout << "Enemies in this location: " << enemiesLeftInLocation << "\n";
        std::cout << randomEnemy.name << " attacks you!\n";

        startCombat(randomEnemy);
    }
}
void Game::chooseLocation() {

    std::cout << "\nChoose location:\n";
    std::cout << "1. Forest\n";
    std::cout << "2. Cave\n";

    int choice = 0;
    if (!readNumber(choice)) { quit(); return; }

    if (choice == 1) {

        std::cout << "You entered the forest.\n";

        Character wolf("Wolf", 60, 12);

        startCombat(wolf);
    }

    else if (choice == 2) {

        std::cout << "You entered the cave.\n";

        std::cout << "A strange man wants to talk...\n";
    }
}
void Game::startCombat(Character e) {
    enemy = e;
    state = GameState::COMBAT;
}

void Game::combatLoop() {
    std::cout << "\n=== COMBAT ===\n";

    while (player.isAlive() && enemy.isAlive()) {
        std::cout << "\n---------------------\n";
        std::cout << "\nPlayer HP: " << player.hp << "\n";
        std::cout << "Enemy: " << enemy.name << " | HP: " << enemy.hp << "\n";
        int escapeChance = 20 + rand() % 41; // 20-60%  Run/Use-item succeed on a random roll vs. their chance; otherwise the turn is wasted.
        int useChance = 30 + rand() % 21;    // 30-50%
        std::cout << "\n[1] Attack\n";
        std::cout << "[2] Run (" << escapeChance << "%)\n";
        std::cout << "[3] Use Item (" << useChance << "%)\n";
        int choice = 0;
        if (!readNumber(choice)) { quit(); return; }
        if (choice < 1 || choice > 3) {
            std::cout << "Choose 1, 2 or 3.\n";
            continue;
        }

        if (choice == 1) {
            const int attack = std::clamp(player.attack, 0, 1000000);
            const int minDamage = std::min(5 + attack / 2, 50);
            const int maxDamage = std::min(10 + attack * 2, 50);

            int damage = minDamage + rand() % (maxDamage - minDamage + 1);

            enemy.takeDamage(damage);

            std::cout << "You hit " << enemy.name
                      << " for " << damage
                      << " damage.\n";
        }
        else if (choice == 2) {
            if (finalBossStarted && enemy.name == "Guardian of the Island") {
                std::cout << "You cannot escape from the final boss!\n";
            }
            else {
                int roll = rand() % 100;

                if (roll < escapeChance) {
                    std::cout << "You escaped successfully!\n";
                    state = GameState::EXPLORATION;
                    return;
                }
                else {
                    std::cout << "Escape failed!\n";
                }
            }
        }

        else if (choice == 3) {

            int roll = rand() % 100;

            if (roll < useChance) {
                useInventoryItem();
                if (state == GameState::EXIT) return;
                continue;
            }
            else {
                std::cout << "You failed to use item in time!\n";
            }
        }

        if (enemy.isAlive()) {
            player.takeDamage(enemy.attack);

            std::cout << enemy.name
                      << " hits you for "
                      << enemy.attack
                      << " damage.\n";
        }
    }

    if (player.isAlive()) {
        std::cout << "\nYou defeated " << enemy.name << "!\n";
        addRandomLoot();
        if (finalBossStarted && enemy.name == "Guardian of the Island") {

            std::cout << "\n";
            std::cout << "========================\n";
            std::cout << "       YOU WON!!\n";
            std::cout << "========================\n";
            std::cout << "\n";

            std::cout << "The island has been conquered.\n";
            std::cout << "Thanks for playing.\n";

            state = GameState::EXIT;
            return;
        }

        enemiesLeftInLocation--;

        if (enemiesLeftInLocation > 0) {             // More enemies may remain in the location; spawn the next one before exploring again.
            Character nextEnemy = createRandomEnemy();

            std::cout << "\nAnother enemy appears!\n";
            std::cout << nextEnemy.name << " attacks you!\n";

            startCombat(nextEnemy);
            return;
        }

        locationsSurvived++;
        state = GameState::EXPLORATION;
        checkWinCondition();
        return;
    }
    else {
        std::cout << "\nYou died!\n";
        std::cout << "Game over.\n";
        state = GameState::EXIT;
        return;
    }

    state = GameState::EXPLORATION;
}

void Game::showInventory() {
    player.inventory.showItems();
}

void Game::save() {
    if (SaveSystem::save(player, locationsSurvived, locationsToWin))
        std::cout << "Game saved.\n";
    else
        std::cout << "Could not save the game.\n";
}
void Game::quit() {
    state = GameState::EXIT;
}

void Game::checkWinCondition() {
    if (locationsSurvived >= locationsToWin && !finalBossStarted) {
        finalBossStarted = true; // Once enough locations are survived, spawn the final boss exactly once.

        std::cout << "\nYou have reached boss!\n";

        int bossHp = 250 + rand() % 101;      // 250-350
        int bossDmg = 20 + rand() % 11;       // 20-30

        Character boss("Guardian of the Island", bossHp, bossDmg);
        startCombat(boss);
    }
}

void Game::restartGame() {
    std::cout << "Restarting game...\n";

    player = Character("Hero", 100, 15);
    enemy = Character("None", 0, 0);
    locationsSurvived = 0;
    locationsToWin = 10;
    enemiesLeftInLocation = 0;
    finalBossStarted = false;
    player.inventory.clear();
    enemy.inventory.clear();
    state = GameState::EXPLORATION;

    player.inventory.addItem(std::make_unique<WeaponItem>("Rusty Sword", 20, 2));
    player.inventory.addItem(std::make_unique<HealingItem>("Small Potion", 15, 20));
    player.inventory.addItem(std::make_unique<ArmorItem>("Wooden Shield", 25, 10));
}
Character Game::createRandomEnemy() { // Returns a random enemy with stats scaled by type (Goblin .. Dark Knight).

    int enemyType = rand() % 6;

    if (enemyType == 0) {
        return Character("Goblin", 40, 8);
    }
    else if (enemyType == 1) {
        return Character("Wolf", 55, 11);
    }
    else if (enemyType == 2) {
        return Character("Skeleton", 70, 13);
    }
    else if (enemyType == 3) {
        return Character("Bandit", 80, 15);
    }
    else if (enemyType == 4) {
        return Character("Orc", 100, 18);
    }
    else {
        return Character("Dark Knight", 130, 22);
    }
}
void Game::useInventoryItem() {
    if (player.inventory.isEmpty()) {
        std::cout << "Inventory is empty.\n";
        return;
    }

    player.inventory.showItems();

    std::cout << "Choose item number: ";

    int index = 0;
    if (!readNumber(index)) { quit(); return; }

    try {
        player.inventory.useItem(index, player);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void Game::clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
