#pragma once
#include <iostream>
#include <sstream>
#include <string>

inline bool readNumber(int& value) {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream input(line);
        if ((input >> value) && (input >> std::ws).eof()) return true;
        std::cout << "Enter a whole number: ";
    }
    return false;
}
