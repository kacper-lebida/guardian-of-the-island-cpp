#include "Character.h"
#include <algorithm>
Character::Character() : name("Unknown"), hp(0), attack(0) {}
Character::Character(std::string n, int h, int a)
    : name(n), hp(h), attack(a) {}

Character::Character(const Character& other)
    : name(other.name), hp(other.hp), attack(other.attack) {}

Character& Character::operator=(const Character& other) {
    if (this != &other) {
        name = other.name;
        hp = other.hp;
        attack = other.attack;
    }

    return *this;
}

void Character::takeDamage(int dmg) {
    hp -= dmg;

    if (hp < 0) { // Clamp HP at 0 so it never goes negative.
        hp = 0;
    }
}

bool Character::isAlive() {
    return hp > 0;
}
void Character::heal(int amount) {
    hp = static_cast<int>(std::clamp(static_cast<long long>(hp) + amount, 0LL, 1000000LL));
}

void Character::increaseAttack(int amount) {
    attack = static_cast<int>(std::clamp(static_cast<long long>(attack) + amount, 0LL, 1000000LL));
}
