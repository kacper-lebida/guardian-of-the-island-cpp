
#pragma once

class Game;

class MenuSystem {
public:
    void showMenu();
    char getInput();
    void handleInput(Game& game, char input);
};
