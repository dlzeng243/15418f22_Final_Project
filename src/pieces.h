#ifndef PIECES_H
#define PIECES_H

#include <string>
#include <map>
#include <vector>

// maps each piece to a specific index
// ZERO INDEX IS EMPTY
std::map<std::string, int> pieces_to_index {{"I2", 1}, {"I3", 2}, {"L3", 3}, {"I4", 4}, {"O4", 5}, {"T4", 6}, {"J4", 7}, {"L4", 8}, {"S4", 9}, {"Z4", 10}};

std::map<int, std::string> index_to_pieces {{0, "00"}, {1, "I2"}, {2, "I3"}, {3, "L3"}, {4, "I4"}, {5, "O4"}, {6, "T4"}, {7, "J4"}, {8, "L4"}, {9, "S4"}, {10, "Z4"}};

// maps an index to a list of rotations
// 0 implies an empty space
// domino I -> 1 -> [[1, 1]] and [[1], [1]]
// tromino I -> 2 -> [[2, 2, 2]] and [[2], [2], [2]]
// tromino L -> 3 -> [[3, 0], [3, 3]], [[0, 3], [3, 3]], [[3, 3], [0, 3]], [[3, 3], [3, 0]]
std::vector<std::vector<std::vector<std::vector<int>>>> index_to_rotations{
    // domino I
    {
        {{1, 1}},
        {{1}, {1}}
    },
    // tromino I
    {
        {{2, 2, 2}},
        {{2}, {2}, {2}}
    },
    // tromino L
    {
        {{3, 0}, {3, 3}}, 
        {{0, 3}, {3, 3}}, 
        {{3, 3}, {0, 3}}, 
        {{3, 3}, {3, 0}}
    },
    // tetromino I
    {
        {{4, 4, 4, 4}},
        {{4}, {4}, {4}, {4}}
    },
    // tetromino O
    {
        {{5, 5}, {5, 5}}
    },
    // tetromino T
    {
        {{0, 6, 0}, {6, 6, 6}}, 
        {{6, 6, 6}, {0, 6, 0}}, 
        {{0, 6}, {6, 6}, {0, 6}}, 
        {{6, 0}, {6, 6}, {6, 0}}
    },
    // tetromino J
    {
        {{7, 0, 0}, {7, 7, 7}}, 
        {{7, 7, 7}, {0, 0, 7}}, 
        {{0, 7}, {0, 7}, {7, 7}}, 
        {{7, 7}, {7, 0}, {7, 0}}
    },
    // tetromino L
    {
        {{0, 0, 8}, {8, 8, 8}}, 
        {{8, 8, 8}, {8, 0, 0}}, 
        {{8, 8}, {0, 8}, {0, 8}}, 
        {{8, 0}, {8, 0}, {8, 8}}
    },
    // tetromino S
    {
        {{0, 9, 9}, {9, 9, 0}}, 
        {{9, 0}, {9, 9}, {0, 9}}
    },
    // tetromino Z
    {
        {{10, 10, 0}, {0, 10, 10}}, 
        {{0, 10}, {10, 10}, {10, 0}}
    }
};

#endif