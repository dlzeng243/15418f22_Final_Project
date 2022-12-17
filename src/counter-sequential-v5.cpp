#include "solver-recursive.h"

// RECURSIVE IMPLEMENTATION BY BLANK SPACES (NO FLOOD FILL) WITH REFERENCES


bool success[8] = {false,false,false,false,false,false,false,false};
BoardTiling solution;

void solver_by_blank_space(BoardTiling board, std::vector<int> pieces){
    solver_sequential(board, pieces, true);
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

    // make sure board can be tiled by the pieces provided mathematically
    check_board();

    // initialize board
    BoardTiling board;
    board.resize(height);
    for(int i = 0; i < height; i++) {
        board[i].resize(width);
    }

    std::vector<int> piece_counts(index_to_pieces.size(), 0);
    for(int x : pieces_index) piece_counts[x]++;

    // do dfs to solve
    Timer t;
    t.reset();
    solver_by_blank_space(board, piece_counts);
    float time = t.elapsed();
    printf("sequential time to solve: %.6fs\n", time);

    // print out board
    printf("Number of solutions: %lld\n", count_sols[0]);
    if (count_sols[0] != 0)
        print_board(solution);
    return 0;
}