#include "solver-recursive.h"

// RECURSIVE IMPLEMENTATION BY BLANK SPACES (NO FLOOD FILL)


bool success[8] = {false,false,false,false,false,false,false,false};
std::vector<std::vector<int>> solution;


void solve_recursive(std::vector<std::vector<int>> board, std::vector<int> pieces) {
    solve_recursive_wrapper(board, 0, pieces);
}

void solve_recursive_wrapper(std::vector<std::vector<int>> board, size_t piece_num, const std::vector<int> &pieces) {
    if (success[0]) return;
    // if we've reached the end of the pieces
    if(piece_num == pieces.size()) {
        solution = board;
        success[0] = true;
        return;
    }
    int next_piece = pieces[piece_num];
    // check if it's valid (can optimize here)
    // at the moment, don't need to do anything

    // get all possible rotations for a given piece
    std::vector<std::vector<std::vector<int>>> &orientations = index_to_rotations[next_piece - 1];

    // iterate through the current board and see if we can place each rotation + position in
    // then put on stack
    for(size_t i = 0; i < orientations.size(); i++) {
        // 2d array of the piece orientation
        std::vector<std::vector<int>> &rotation = orientations[i];
        // we made rotation, so we know rotation is not degenerate
        int h_len = (int)rotation.size();
        int w_len = (int)rotation[0].size();
        // try placing piece in bottom left spot
        for(int h = 0; h < height - h_len + 1; h++) {
            for(int w = 0; w < width - w_len + 1; w++) {
                // check if any spots in the piece size is taken up
                int check = 0;
                // at the moment, just go through the whole subarray
                // could probably optimize in the future
                for(int a = 0; a < h_len; a++) {
                    for(int b = 0; b < w_len; b++) {
                        // position is an empty space
                        if(rotation[a][b] == 0) {
                            continue;
                        }
                        // position is not an empty space - need to check if it's filled in the board
                        else if (board[h + a][w + b] > 0) {
                            check++;
                        }
                    }
                }
                // only way this occurs is if all positions checked are empty
                if(check == 0) {
                    std::vector<std::vector<bool>> borderings = std::vector<std::vector<bool>>(h_len+2, std::vector<bool>(w_len+2, false));
                    auto board_copy = board;
                    for(int a = 0; a < h_len; a++) {
                        for(int b = 0; b < w_len; b++) {
                            if(rotation[a][b] == 0) {
                                continue;
                            }
                            for(int e = 0; e <= 2; e++)
                                for(int f = 0; f <= 2; f++) {
                                    if (e == 1 && f == 1) continue;
                                    int dx = e - 1;
                                    int dy = f - 1;
                                    if (a + dx < 0 || b + dy < 0
                                        || a + dx >= h_len || b + dy >= w_len
                                        || rotation[a+dx][b+dy] == 0)
                                        borderings[a+e][b+f] = true;
                                }
                            board_copy[h + a][w + b] = rotation[a][b];
                        }
                    }
                    std::vector<std::pair<int,int>> borders;
                    for(int i = 0; i < h_len+2; i++) {
                        for(int j = 0; j < w_len+2; j++) {
                            if (borderings[i][j] == true && h + i - 1 >= 0 && h+i-1 < height
                                && w+j-1 >= 0 && w+j-1 < width)
                                borders.push_back(std::make_pair(h+i-1, w+j-1));
                        }
                    }
                    std::vector<int> sec_sizes = flood_fill(board_copy, borders);
                    bool valid = true;
                    for(int x : sec_sizes) {
                        if (x == 1) valid = false;
                    }
                    if (valid)
                    {
                    solve_recursive_wrapper(board_copy, piece_num+1, pieces);
                    }
                }
            }
        }
    }
}


void solver_by_blank_space(std::vector<std::vector<int>> board, std::vector<int> pieces) {
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
    for(int i = 1; i < pieces.size(); i++) {
        if (pieces[i] == 0) continue;
        for(const auto &piece : index_to_rotations[i])
        {
            int offset = 0; while (piece[0][offset] == 0) offset++;
            if (place_piece(board_copy, row, col-offset, piece)) {
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
    std::vector<std::vector<int>> board;
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