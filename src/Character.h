#pragma once
#include <string>
#include "Inventory.h"

class Character { // Player/enemy entity: core stats plus its own inventory.
public:
    std::string name;
    int hp;
    int attack;

    Inventory inventory;

    Character(std::string n, int h, int a);
    Character();
    Character(const Character& other);
    Character& operator=(const Character& other);


    void takeDamage(int dmg);
    bool isAlive();
    void heal(int amount);
    void increaseAttack(int amount);
};
