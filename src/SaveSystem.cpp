#include "SaveSystem.h"
#include <fstream>
#include <limits>
#include <stdexcept>
#include <utility>

bool SaveSystem::save(const Character& player, int survived, int goal) {
    std::ofstream file("save.txt");
    file << player.name << "\n" << player.hp << "\n" << player.attack << "\n"
         << survived << "\n" << goal << "\n";
    player.inventory.saveToFile(file);
    file.close();
    return static_cast<bool>(file);
}

bool SaveSystem::load(Character& player, int& survived, int& goal) {
    std::ifstream file("save.txt");
    if (!file) return false;
    Character candidate;
    int progress = 0, target = 0;
    if (!std::getline(file, candidate.name) || candidate.name.empty() ||
        candidate.name.size() > 100 ||
        !(file >> candidate.hp >> candidate.attack >> progress >> target) ||
        candidate.hp <= 0 || candidate.hp > 1000000 ||
        candidate.attack < 0 || candidate.attack > 1000000 ||
        target <= 0 || target > 1000000 || progress < 0 || progress > target)
        return false;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    try {
        candidate.inventory.loadFromFile(file);
    } catch (const std::runtime_error&) {
        return false;
    }
    file >> std::ws;
    if (!file.eof()) return false;
    // Commit only after the entire save has been validated.
    player = candidate;
    player.inventory = std::move(candidate.inventory);
    survived = progress;
    goal = target;
    return true;
}
