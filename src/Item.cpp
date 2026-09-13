#include "Item.h"
#include "Character.h"
#include <iostream>

Item::Item(std::string n, int v)
    : name(n), value(v) {}

std::string Item::getName() const {
    return name;
}

int Item::getValue() const {
    return value;
}

WeaponItem::WeaponItem(std::string n, int v, int bonus)
    : Item(n, v), attackBonus(bonus) {}

void WeaponItem::use(Character& player) {
    player.increaseAttack(attackBonus);
    std::cout << "Equipped " << name << ". Attack +" << attackBonus << "\n";
}

std::string WeaponItem::getDescription() const {
    return name + " (Attack +" + std::to_string(attackBonus) + ")";
}

HealingItem::HealingItem(std::string n, int v, int heal)
    : Item(n, v), healAmount(heal) {}

void HealingItem::use(Character& player) {
    player.heal(healAmount);
    std::cout << "Used " << name << ". HP +" << healAmount << "\n";
}

std::string HealingItem::getDescription() const {
    return name + " (Heal +" + std::to_string(healAmount) + " HP)";
}

ArmorItem::ArmorItem(std::string n, int v, int hp)
    : Item(n, v), hpBonus(hp) {}

void ArmorItem::use(Character& player) {
    player.heal(hpBonus);
    std::cout << "Used " << name << ". Bonus HP +" << hpBonus << "\n";
}

std::string ArmorItem::getDescription() const {
    return name + " (Bonus HP +" + std::to_string(hpBonus) + ")";
}

std::string WeaponItem::getType() const {
    return "WEAPON";
}

int WeaponItem::getPower() const {
    return attackBonus;
}

std::string HealingItem::getType() const {
    return "HEALING";
}

int HealingItem::getPower() const {
    return healAmount;
}

std::string ArmorItem::getType() const {
    return "ARMOR";
}

int ArmorItem::getPower() const {
    return hpBonus;
}
