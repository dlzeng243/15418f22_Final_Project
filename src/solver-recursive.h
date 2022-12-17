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
bool switched = false;
extern bool success[8];
extern BoardTiling solution;
long long num_sols = 0;
long long count_sols[1024];

double thread_times[1024];
Timer timers[1024];

// file to load
std::string file = "";

// pieces to try to use to tile a board
std::vector<int> pieces_index;

// useful functions:

// dfs to solve the board

void solve_recursive(BoardTiling board, std::vector<int> pieces);

void solve_recursive_wrapper(BoardTiling board, size_t piece_num, const std::vector<int> &pieces);

// returns true and modifies board to place piece at row, col if we can place piece
// otherwise, returns false with no modifications
inline bool place_piece(BoardTiling &board, int row, int col, const BoardTiling &piece) {
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
            board[row + a][col + b] ^= piece[a][b];
        }
    }
    return true;
}

//removes a piece from the board. assumes it exists already.
inline void unplace_piece(BoardTiling &board, int row, int col, const BoardTiling &piece) {
    int h_len = (int)piece.size();
    int w_len = (int)piece[0].size();
    for(int a = 0; a < h_len; a++) {
        for(int b = 0; b < w_len; b++) {
            board[row + a][col + b] ^= piece[a][b];
        }
    }
}

void counting_sequential_wrapper(BoardTiling &board, std::vector<int> &pieces, bool initial=false) {
    //find the first row where there's a blank square, and the first blank square in that row
    if (initial) timers[8 * omp_get_thread_num()].reset();
    int row = -1;
    int col = -1;
    for(int r = 0; r < height; r++) {
        for(int c = 0; c < width; c++) {
            if (board[r][c] != 0) continue;
            //put a piece in that spot.
            row = r; col = c;
            break;
        }
        if (row != -1) break;
    }
    if (row == -1) {
        ///TODO: put a lock around this section
        if (!success[0]) {
            solution = board;
            success[0] = true;
        }
        count_sols[8 * omp_get_thread_num()]++;
    }
    //iterate backwards starting with bigger pieces
    else for(size_t i = pieces.size() - 1; i >= 1; i--) {
        if (pieces[i] == 0) continue;
        for(const auto &piece : index_to_rotations[i])
        {
            int offset = 0; while (piece[0][offset] == 0) offset++;
            if (place_piece(board, row, col-offset, piece)) {
                pieces[i]--;
                counting_sequential_wrapper(board, pieces);
                // reset board/state
                pieces[i]++;
                unplace_piece(board, row, col-offset, piece);
            }
        }
    }
    if (initial) thread_times[8 * omp_get_thread_num()] += timers[8 * omp_get_thread_num()].elapsed();
}

void solver_sequential_wrapper(BoardTiling &board, std::vector<int> &pieces) {
    if (success[0]) return;
    //find the first row where there's a blank square, and the first blank square in that row
    int row = -1;
    int col = -1;
    for(int r = 0; r < height; r++) {
        for(int c = 0; c < width; c++) {
            if (board[r][c] != 0) continue;
            //put a piece in that spot.
            row = r; col = c;
            break;
        }
        if (row != -1) break;
    }
    if (row == -1) {
        ///TODO: put a lock around this section
        if (!success[0]) {
            solution = board;
            success[0] = true;
        }
    }
    //iterate backwards starting with bigger pieces
    else for(size_t i = pieces.size() - 1; i >= 1; i--) {
        if (pieces[i] == 0) continue;
        for(const auto &piece : index_to_rotations[i])
        {
            int offset = 0; while (piece[0][offset] == 0) offset++;
            if (place_piece(board, row, col-offset, piece)) {
                pieces[i]--;
                solver_sequential_wrapper(board, pieces);
                // reset board/state
                pieces[i]++;
                unplace_piece(board, row, col-offset, piece);
            }
        }
    }
}

void solver_sequential(BoardTiling board, std::vector<int> pieces, bool counting=false) {
    if (counting) counting_sequential_wrapper(board, pieces, true);
    else solver_sequential_wrapper(board, pieces);
}

std::vector<int> flood_fill(const BoardTiling &board, const std::vector<std::pair<int,int>> &ijs) {
    std::vector<int> sec_sizes;
    int M = board.size(), N = board[0].size();
    std::vector<int> visited(M * N, -1);
    auto visited_at = [=,&visited](const std::pair<int,int> p) {return visited[p.first * N + p.second];};
    auto visit_loc = [=,&visited](const std::pair<int,int> p, int value) {visited[p.first * N + p.second] = value;};

    for(size_t idx = 0; idx < ijs.size(); idx++) {
        if (ijs[idx].first < 0 || ijs[idx].first >= M
            || ijs[idx].second < 0 || ijs[idx].second >= N
            || board[ijs[idx].first][ijs[idx].second] != 0) {
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
    if ((switched = (height < width))) {
        int tmp = width;
        width = height;
        height = tmp;
    }
    assert(width <= height);
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
        if(pieces_index[i] < 2) {
            sum += 1;
        }
        else if(pieces_index[i] < 3) {
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

inline void print_board(const BoardTiling &b) {
    if (!switched) {
        for(size_t i = 0; i < b.size(); i++) {
            for(size_t j = 0; j < b[0].size(); j++) {
                std::cout << std::setw(4) <<index_to_pieces[b[i][j]] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    else {
        for(size_t j = 0; j < b[0].size(); j++) {
            for(size_t i = 0; i < b.size(); i++) {
                std::cout << std::setw(4) <<index_to_pieces[b[i][j]] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

#endif