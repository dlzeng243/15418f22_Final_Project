#include "solver-stack.h"

// STACK IMPLEMENTATION WITH FLOOD FILL
bool success[8] = {false,false,false,false,false,false,false,false};

BoardTiling solve(BoardTiling board, std::vector<int> pieces) {
    // stack of board states
    std::stack<std::pair<BoardTiling, size_t>> st;
    st.push(std::make_pair(board, 0));
    while(!st.empty()) {
        // get next board
        auto b = st.top();
        st.pop();
        // if we've reached the end of the pieces
        if(b.second == pieces.size()) {
            success[0] = true;
            return b.first;
        }
        int next_piece = pieces[b.second];
        auto &curr_board = b.first;
        // print_board(curr_board);
        // check if it's valid (can optimize here)
        // at the moment, don't need to do anything

        // get all possible rotations for a given piece
        const std::vector<BoardTiling> &orientations = index_to_rotations[next_piece];

        // iterate through the current board and see if we can place each rotation + position in
        // then put on stack
        for(size_t i = 0; i < orientations.size(); i++) {
            // 2d array of the piece orientation
            const BoardTiling &rotation = orientations[i];
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
                            else if (curr_board[h + a][w + b] > 0) {
                                check++;
                            }
                        }
                    }
                    // only way this occurs is if all positions checked are empty
                    if(check == 0) {
                        std::vector<std::vector<bool>> borderings = std::vector<std::vector<bool>>(h_len+2, std::vector<bool>(w_len + 2, false));
                        auto board_copy = curr_board;
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
                                            || rotation[a + dx][b + dy] == 0)
                                            borderings[a + e][b + f] = true;
                                    }
                                board_copy[h + a][w + b] = rotation[a][b];
                            }
                        }
                        std::vector<std::pair<int,int>> borders;
                        for(int i = 0; i < h_len + 2; i++) {
                            for(int j = 0; j < w_len + 2; j++) {
                                if (borderings[i][j] == true && h + i - 1 >= 0 && h + i - 1 < height
                                                             && w + j - 1 >= 0 && w + j - 1 < width)
                                    borders.push_back(std::make_pair(h + i - 1, w + j - 1));
                            }
                        }
                        std::vector<int> sec_sizes = flood_fill(board_copy, borders);
                        bool valid = true;
                        for(int x : sec_sizes) {
                            if (x == 1) valid = false;
                        }
                        if (valid)
                            st.push(std::make_pair(board_copy, b.second + 1));
                    }
                }
            }
        }
    }
    return {};
}
/*
-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 
-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 
-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 
-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 
height = 4
width = 10
if we have a 2x2 O piece
want to iterate from h = 0 to h = 1 -> h < height - h_len + 1
want to iterate from w = 0 to w = 8 -> w < width - w_len + 1

*/

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
    check_board();

    // initialize board
    BoardTiling board;
    board.resize(height);
    for(int i = 0; i < height; i++) {
        board[i].resize(width);
    }

    // do dfs to solve
    Timer t;
    t.reset();
    auto final_board = solve(board, pieces_index);
    float time = t.elapsed();
    printf("sequential time to solve: %.6fs\n", time);

    // print out board
    if (success[0])
        print_board(final_board);
    else
        printf("no solution found\n");
    return 0;
}