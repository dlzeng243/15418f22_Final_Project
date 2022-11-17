#ifndef SOLVER_H
#define SOLVER_H

#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// variables:

// width and height of the board we want to solve
// defaults to a tetris board
int width = 10;
int height = 4;

// file to load
std::string file = "";

// pieces to try to use to tile a board
std::vector<std::string> pieces;

// useful functions:
// loads pieces from a file into a vector
// file is structured as one piece per line, in the format <letter><size>
inline bool loadFromFile(std::string fileName) {
    std::ifstream f(fileName);
    assert((bool)f && "Cannot open input file");

    std::string line;
    while (std::getline(f, line)) {
        pieces.push_back(line);
    }
    return true;
}

#endif