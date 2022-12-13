#include "solver-stack.h"

// STACK IMPLEMENTATION WITH NOTHING

std::vector<std::vector<int>> solve(std::vector<std::vector<int>> board, std::vector<int> pieces) {
    // stack of board states
    std::stack<std::pair<std::vector<std::vector<int>>, size_t>> st;
    st.push(std::make_pair(board, 0));
    while(!st.empty()) {
        // get next board
        auto b = st.top();
        st.pop();
        // if we've reached the end of the pieces
        if(b.second == pieces.size()) {
            return b.first;
        }
        int next_piece = pieces[b.second];
        auto &curr_board = b.first;
        // print_board(curr_board);
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
            auto board_copy = curr_board;
            // try placing piece in bottom left spot
            for(int h = 0; h < height - h_len + 1; h++) {
                for(int w = 0; w < width - w_len + 1; w++) {
                    if(place_piece(board_copy, h, w, rotation)) {
                        st.push(std::make_pair(board_copy, b.second + 1));
                        board_copy = curr_board;
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
    std::sort(pieces_index.begin(), pieces_index.end());

    // make sure board can be tiled by the pieces provided mathematically
    int sum = 0;
    for(size_t i = 0; i < pieces_index.size(); i++) {
        // std::cout << pieces_index[i] << "\n";
        if(pieces_index[i] < 1) {
            sum += 2;
        }
        else if(pieces_index[i] < 3) {
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

    // do dfs to solve
    Timer t;
    t.reset();
    auto final_board = solve(board, pieces_index);
    float time = t.elapsed();
    printf("sequential time to solve: %.6fs\n", time);

    // print out board
    print_board(final_board);
    return 0;
}