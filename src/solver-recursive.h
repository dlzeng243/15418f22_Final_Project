#ifndef SOLVERP_H
#define SOLVERP_H

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
#include <omp.h>
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

void solve_recursive(std::vector<std::vector<int>> board, std::vector<int> pieces);

void solve_recursive_wrapper(std::vector<std::vector<int>> board, size_t piece_num, const std::vector<int> &pieces);

// returns true and modifies board to place piece at row, col if we can place piece
// otherwise, returns false with no modifications
inline bool place_piece(std::vector<std::vector<int>> &board, int row, int col, const std::vector<std::vector<int>> &piece) {
    // check if any spots in the piece size is taken up
    int h_len = (int)piece.size();
    int w_len = (int)piece[0].size();
    // at the moment, just go through the whole subarray
    // could probably optimize in the future
    if (row < 0 || col < 0 || row + h_len > height || col + w_len > width) return false;
    for(int a = 0; a < h_len; a++) {
        for(int b = 0; b < w_len; b++) {
            // position is an empty space, so move next
            if(piece[a][b] == 0) {
                continue;
            }
            // position is not an empty space - need to check if it's filled in the board
            else if (board[row + a][col + b] > 0) {
                return false;
            }
        }
    }
    // only way this occurs is if all positions checked are empty
    for(int a = 0; a < h_len; a++) {
        for(int b = 0; b < w_len; b++) {
            if(piece[a][b] == 0) {
                continue;
            }
            board[row + a][col + b] = piece[a][b];
        }
    }
    return true;
}

std::vector<int> flood_fill(const std::vector<std::vector<int>> &board, const std::vector<std::pair<int,int>> &ijs) {
    std::vector<int> sec_sizes;
    int M = board.size(), N = board[0].size();
    std::vector<int> visited(M * N, -1);
    auto visited_at = [=,&visited](const std::pair<int,int> p) {return visited[p.first * N + p.second];};
    auto visit_loc = [=,&visited](const std::pair<int,int> p, int value) {visited[p.first * N + p.second] = value;};

    for(size_t idx = 0; idx < ijs.size(); idx++) {
        if (board[ijs[idx].first][ijs[idx].second] != 0) {
            sec_sizes.push_back(0);
            continue;
        }
        if (visited_at(ijs[idx]) != -1) {
            sec_sizes.push_back(sec_sizes[visited_at(ijs[idx])]);
            continue;
        }
        int section_size = 0;
        std::stack<std::pair<int, int>> st;
        st.push(ijs[idx]);
        while(!st.empty()) {
            auto coords = st.top();
            st.pop();
            if (coords.first < 0 || coords.first >= M
                || coords.second < 0 || coords.second >= N)
                continue;
            if (visited_at(coords) != -1) continue;
            if (board[coords.first][coords.second] != 0) continue;
            visit_loc(coords, idx);
            section_size++;
            st.push(std::make_pair(coords.first+1, coords.second));
            st.push(std::make_pair(coords.first-1, coords.second));
            st.push(std::make_pair(coords.first, coords.second+1));
            st.push(std::make_pair(coords.first, coords.second-1));
        }
        sec_sizes.push_back(section_size);
    }
    return sec_sizes;
}

// loads pieces from a file into a vector
// file is structured as one piece per line, in the format <letter><size>
inline bool load_from_file(std::string fileName) {
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

inline void check_board() {
    int sum = 0;
    for(size_t i = 0; i < pieces_index.size(); i++) {
        // std::cout << pieces_index[i] << "\n";
        if(pieces_index[i] < 2) {
            sum += 1;
        }
        if(pieces_index[i] < 3) {
            sum += 2;
        }
        else if(pieces_index[i] < 5) {
            sum += 3;
        }
        else if(pieces_index[i] < 12) {
            sum += 4;
        }
        else {
            sum += 5;
        }
    }
    assert(sum == width * height);
}

inline void print_board(std::vector<std::vector<int>> b) {
    for(size_t i = 0; i < b.size(); i++) {
        for(size_t j = 0; j < b[0].size(); j++) {
            std::cout << std::setw(4) <<index_to_pieces[b[i][j]] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

#endif