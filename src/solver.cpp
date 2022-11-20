#include "solver.h"

std::vector<std::vector<std::string>> solve(std::vector<std::vector<std::string>> board, std::vector<std::string> pieces) {
    // stack of board states
    std::stack<std::pair<std::vector<std::vector<std::string>>, size_t>> st;
    st.push(std::make_pair(board, 0));
    while(!st.empty()) {
        // get next board
        auto b = st.top();
        st.pop();
        // if we've reached the end of the pieces
        if(b.second == pieces.size()) {
            return b.first;
        }
        std::string next_piece = pieces[b.second];

        // check if it's valid (can optimize here)
        // at the moment, don't need to do anything

        // get all possible rotations for a given piece

        // iterate through the current board and see if we can place each rotation + position in
        // then put on stack
    }
    return {};
}


int main(int argc, char** argv) {
    // read command line arguments
    for (int i = 1; i < argc; i++) {
        // sets the width
        if (strcmp(argv[i], "--width") == 0)
            width = atoi(argv[i + 1]);
        // sets the height
        else if (strcmp(argv[i], "--height") == 0)
            height = atoi(argv[i + 1]);
        // sets the file name to read the pieces from
        else if (strcmp(argv[i], "--file") == 0)
            file = argv[i + 1];
    }
    // read pieces from file
    loadFromFile(file);
    // sort so we have pieces going in consecutive order
    std::sort(pieces_index.begin(), pieces_index.end());

    // make sure board can be tiled by the pieces provided mathematically
    int sum = 0;
    for(size_t i = 0; i < pieces_index.size(); i++) {
        if(pieces_index[i] < 1) {
            sum += 2;
        }
        else if(pieces_index[i] < 3) {
            sum += 3;
        }
        // at the moment, only contain up to tetrominos
        else {
            sum += 4;
        }
    }
    assert(sum == width * height);

    // initialize board
    std::vector<std::vector<int>> board;
    board.resize(height);
    for(int i = 0; i < height; i++) {
        board[i].resize(width);
        for(int j = 0; j < width; j++) {
            board[i][j] = -1;
        }
    }

    // do dfs to solve

    // print out board
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
    return 0;
}