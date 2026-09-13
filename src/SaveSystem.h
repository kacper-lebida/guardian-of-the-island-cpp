#pragma once
#include "Character.h"

// Static helpers: persist/restore the whole game state to save.txt.

class SaveSystem {
public:
    static bool save(const Character& player, int survived, int goal);
    static bool load(Character& player, int& survived, int& goal);
};
