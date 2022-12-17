#include "solver-recursive.h"

// RECURSIVE IMPLEMENTATION BY BLANK SPACES (NO FLOOD FILL)


bool success[8] = {false,false,false,false,false,false,false,false};
BoardTiling solution;

void solver_by_blank_space(BoardTiling board, std::vector<int> pieces) {
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
                {
                    #pragma omp task final(row >= width-2)
                    solver_by_blank_space(board, pieces);
                }
                // reset board/state
                pieces[i]++;
                unplace_piece(board, row, col-offset, piece);
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
    check_board();

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
    #pragma omp parallel
    #pragma omp single
    {
        #pragma omp task mergeable
        solver_by_blank_space(board, piece_counts);
    }
    float time = t.elapsed();
    printf("sequential time to solve: %.6fs\n", time);

    // print out board
    if (success[0])
        print_board(solution);
    else
        printf("no solution found\n");
    return 0;
}