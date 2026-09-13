#include "CombatSystem.h"
#include <iostream>

void CombatSystem::fight(Character& a, Character& b) {
    std::cout << "Fight: " << a.name << " vs " << b.name << "\n";
}
