#ifndef PIECES_H
#define PIECES_H

#include <string>
#include <map>
#include <vector>

// maps each piece to a specific index
std::map<std::string, int> pieces_to_index {{"I2", 0}, {"I3", 1}, {"L3", 2}, {"I4", 3}, {"O4", 4}, {"T4", 5}, {"J4", 6}, {"L4", 7}, {"S4", 8}, {"Z4", 9}};

// maps an index to a list of rotations
// -1 implies an empty space
// domino I -> 0 -> [[0, 0]] and [[0], [0]]
// tromino I -> 1 -> [[1, 1, 1]] and [[1], [1], [1]]
// tromino L -> 2 -> [[2, -1], [2, 2]], [[-1, 2], [2, 2]], [[2, 2], [-1, 2]], [[2, 2], [2, -1]]
std::vector<std::vector<std::vector<std::vector<int>>>> index_to_rotations{
    // domino I
    {
        {{0, 0}},
        {{0}, {0}}
    },
    // tromino I
    {
        {{1, 1, 1}},
        {{1}, {1}, {1}}
    },
    // tromino L
    {
        {{2, -1}, {2, 2}}, 
        {{-1, 2}, {2, 2}}, 
        {{2, 2}, {-1, 2}}, 
        {{2, 2}, {2, -1}}
    },
    // tetromino I
    {
        {{3, 3, 3, 3}},
        {{3}, {3}, {3}, {3}}
    },
    // tetromino O
    {
        {{4, 4}, {4, 4}}
    },
    // tetromino T
    {
        {{-1, 5, -1}, {5, 5, 5}}, 
        {{5, 5, 5}, {-1, 5, -1}}, 
        {{-1, 5}, {5, 5}, {-1, 5}}, 
        {{5, -1}, {5, 5}, {5, -1}}
    },
    // tetromino J
    {
        {{6, -1, -1}, {6, 6, 6}}, 
        {{6, 6, 6}, {-1, -1, 6}}, 
        {{-1, 6}, {-1, 6}, {6, 6}}, 
        {{6, 6}, {6, -1}, {6, -1}}
    },
    // tetromino L
    {
        {{-1, -1, 7}, {7, 7, 7}}, 
        {{7, 7, 7}, {7, -1, -1}}, 
        {{7, 7}, {-1, 7}, {-1, 7}}, 
        {{7, -1}, {7, -1}, {7, 7}}
    },
    // tetromino S
    {
        {{-1, 8, 8}, {8, 8, -1}}, 
        {{8, -1}, {8, 8}, {-1, 8}}
    },
    // tetromino Z
    {
        {{9, 9, -1}, {-1, 9, 9}}, 
        {{-1, 9}, {9, 9}, {9, -1}}
    }
};

#endif