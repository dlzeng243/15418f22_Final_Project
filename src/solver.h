#ifndef SOLVER_H
#define SOLVER_H

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include "pieces.h"
#include "timing.h"

// variables:

// width and height of the board we want to solve
// defaults to a tetris board
int width = 10;
int height = 4;

// file to load
std::string file = "";

// pieces to try to use to tile a board
std::vector<int> pieces_index;

// useful functions:

// dfs to solve the board
std::vector<std::vector<std::string>> solve();


// loads pieces from a file into a vector
// file is structured as one piece per line, in the format <letter><size>
inline bool loadFromFile(std::string fileName) {
    std::ifstream f(fileName);
    // make sure we can read the file
    assert((bool)f && "Cannot open input file");
    std::vector<std::string> pieces;
    std::string line;
    // first line is height and width
    std::getline(f, line);
    size_t pos = line.find(" ");
    height = atoi((line.substr(0, pos).c_str()));
    width = atoi((line.substr(pos + 1, line.size() - pos).c_str()));
    // std::cout << "height: " << height << " width: " << width << "\n";
    // rest of the file are pieces
    while (std::getline(f, line)) {
        pieces.push_back(line);
    }
    pieces_index.resize(pieces.size());
    for(size_t i = 0; i < pieces.size(); i++) {
        pieces_index[i] = pieces_to_index[pieces[i]];
    }
    return true;
};

inline void print_board(std::vector<std::vector<int>> b) {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            std::cout  << std::setw(4) << index_to_pieces[b[i][j]] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

#endif