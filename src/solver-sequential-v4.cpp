#include "solver-recursive.h"

// RECURSIVE IMPLEMENTATION BY BLANK SPACES + FLOOD FILL

bool success[8] = {false,false,false,false,false,false,false,false};
BoardTiling solution;

void solver_by_blank_space(BoardTiling board, std::vector<int> pieces) {
    if (success[0]) return;
    //find the first row where there's a blank square, and the first blank square in that row
    auto board_copy = board;
    int row = -1;
    int col = -1;
    for(int r = 0; r < height; r++) {
        for(int c = 0; c < width; c++) {
            if (board_copy[r][c] != 0) continue;
            //put a piece in that spot.
            row = r; col = c;
            break;
        }
        if (row != -1) break;
    }
    if (row == -1) {
        solution = board;
        success[0] = true;
        return;
    }
    for(size_t i = 1; i < pieces.size(); i++) {
        if (pieces[i] == 0) continue;
        for(size_t j = 0; j < index_to_rotations[i].size(); j++)
        {
            const auto &piece = index_to_rotations[i][j];
            int offset = 0; while (piece[0][offset] == 0) offset++;
            if (place_piece(board_copy, row, col-offset, piece)) {
                auto borders = pieces_borders[i][j];
                for (auto &pair : borders) {
                    pair.first += row;
                    pair.second += col - offset ;
                }
                const auto section_sizes = flood_fill(board_copy, borders);
                for(int x : section_sizes) if (x == 1) {
                    board_copy = board; continue;
                }
                pieces[i]--;
                solver_by_blank_space(board_copy, pieces);
                // reset board/state
                pieces[i]++;
                board_copy = board;
            }
        }
    }
}

int main(int argc, char** argv) {
    // read command line arguments
    for (int i = 1; i < argc; i++) {
        // sets the file name to read the pieces from
        if (strcmp(argv[i], "--file") == 0)
            file = argv[i + 1];
    }
    // not a degenerate board
    assert(width > 0);
    assert(height > 0);
    // read pieces from file
    load_from_file(file);
    // sort so we have pieces going in consecutive order
    std::sort(pieces_index.rbegin(), pieces_index.rend());

    // make sure board can be tiled by the pieces provided mathematically
    int sum = 0;
    for(size_t i = 0; i < pieces_index.size(); i++) {
        // std::cout << pieces_index[i] << "\n";
        if(pieces_index[i] < 2) {
            sum += 2;
        }
        else if(pieces_index[i] < 4) {
            sum += 3;
        }
        // at the moment, only contain up to tetrominos
        else if(pieces_index[i] < 11) {
            sum += 4;
        }
        else {
            sum += 5;
        }
    }
    assert(sum == width * height);

    // initialize board
    BoardTiling board;
    board.resize(height);
    for(int i = 0; i < height; i++) {
        board[i].resize(width);
    }

    std::vector<int> piece_counts(29, 0);
    for(int x : pieces_index) piece_counts[x]++;

    // do dfs to solve
    Timer t;
    t.reset();
    solver_by_blank_space(board, piece_counts);
    float time = t.elapsed();
    printf("sequential time to solve: %.6fs\n", time);

    // print out board
    if (success[0])
        print_board(solution);
    else
        printf("no solution found\n");
    return 0;
}