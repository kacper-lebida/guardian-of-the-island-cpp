#pragma once
#include "Character.h"
#include "MenuSystem.h"
#include <vector>
#include <string>


// Top-level state machine driving the main loop in run().
enum class GameState {
    EXPLORATION,
    COMBAT,
    EXIT
};

class Game {
private:
    Character player;
    Character enemy;
    Character createRandomEnemy();

    MenuSystem menu;
    GameState state;
    
    int locationsSurvived; // progress toward th boss fight
    int locationsToWin; // locations needed before the final boss
    int enemiesLeftInLocation; // remaining enemies in the current location
    
    std::vector<std::string> locations;
    bool finalBossStarted; // ensures the boss spawns only once
    

public:
    Game();
    void run();

    void explore();
    void startCombat(Character e);
    void combatLoop();
    void randomEvent();
    void chooseLocation();

    void showInventory();
    void save();
    void quit();
    void checkWinCondition();
    void restartGame();
    void useInventoryItem();
    void clearConsole();
    void addRandomLoot();

    
};
