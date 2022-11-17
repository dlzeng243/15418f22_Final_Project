#include "solver.h"

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

    // make sure board can be tiled by the pieces provided mathematically
    int sum = 0;
    for(size_t i = 0; i < pieces.size(); i++) {
        sum += (int)(pieces[i].at(1) - '0');
    }
    assert(sum == width * height);

    // initialize board
    std::vector<std::vector<std::string>> board;
    board.resize(height);
    for(int i = 0; i < height; i++) {
        board[i].resize(width);
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