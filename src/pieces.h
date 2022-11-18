#ifndef PIECES_H
#define PIECES_H

#include <string>
#include <map>
// domino
std::map<std::string, int> two_map{{"I", 0}};
// tromino
std::map<std::string, int> three_map{{"I", 0}, {"L", 1}};
// tetramino
std::map<std::string, int> four_map{{"I", 0}, {"O", 1}, {"T", 2}, {"J", 3}, {"L", 4}, {"S", 5}, {"Z", 6}};

#endif