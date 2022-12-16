#include "solver-stack.h"
#include <random>

// print borders into a data structure
void print_borders() {
    std::cout << "std::vector<std::vector<std::pair<int,int>>> borders {\n";
    std::cout << "\t{ },\n";
    for(size_t i = 1; i < index_to_rotations.size(); i++) {
        std::cout << "\t{\n";
        const std::vector<std::vector<std::vector<int>>> &orientations = index_to_rotations[i];
        for(size_t j = 0; j < orientations.size(); j++) {
            const std::vector<std::vector<int>> &piece = orientations[j];
            int h_len = (int)piece.size();
            int w_len = (int)piece[0].size();
            std::vector<std::vector<bool>> borderings = std::vector<std::vector<bool>>(h_len+2, std::vector<bool>(w_len + 2, false));
            std::cout << "\t\t{\t";
            for(int a = 0; a < h_len; a++) {
                for(int b = 0; b < w_len; b++) {
                    if(piece[a][b] == 0) {
                        continue;
                    }
                    for(int e = 0; e <= 2; e++) {
                        for(int f = 0; f <= 2; f++) {
                            if (e == 1 && f == 1) continue;
                            int dx = e - 1;
                            int dy = f - 1;
                            if (a + dx < 0 || b + dy < 0
                                || a + dx >= h_len || b + dy >= w_len
                                || piece[a + dx][b + dy] == 0) {
                                borderings[a + e][b + f] = true;
                                // std::cout << "\t\t\t{" + std::to_string(a + dx) + ", " + std::to_string(b + dy) + "},\n";
                            }
                        }
                    }
                }
            }
            for(int a = 0; a < (int)borderings.size(); a++) {
                for(int b = 0; b < (int)borderings[0].size(); b++) {
                    if(borderings[a][b]) {
                        std::cout << "{" + std::to_string(a - 1) + ", " + std::to_string(b - 1) + "}, ";
                    }
                }
            }
            std::cout << "\t},\n";
        }
        std::cout << "\t},\n";
    }
    std::cout << "}\n";
}

// visualize above
void visual_borders() {
    for(size_t i = 1; i < index_to_rotations.size(); i++) {
        const std::vector<std::vector<std::vector<int>>> &orientations = index_to_rotations[i];
        for(size_t j = 0; j < orientations.size(); j++) {
            const std::vector<std::vector<int>> &piece = orientations[j];
            int h_len = (int)piece.size();
            int w_len = (int)piece[0].size();
            std::vector<std::vector<int>> borderings = std::vector<std::vector<int>>(h_len+2, std::vector<int>(w_len + 2, 0));
            std::vector<std::pair<int,int>> bi = pieces_borders[i][j];
            for(size_t k = 0; k < bi.size(); k++) {
                borderings[bi[k].first + 1][bi[k].second + 1] = 1;
            }
            for(size_t a = 0; a < borderings.size(); a++) {
                for(size_t b = 0; b < borderings[0].size(); b++) {
                    std::cout  << std::setw(4) << borderings[a][b] << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }
}

// 
void randomize_board(int h, int w, int seed) {
    int sum = h * w;
    // Mersenne Twister random engine:
    std::mt19937 urbg {static_cast<unsigned int>(seed)};  
    std::uniform_int_distribution<int> pentamino {1, 29};
    std::uniform_int_distribution<int> tetramino {1, 11};
    std::uniform_int_distribution<int> trimino {1, 4};
    std::uniform_int_distribution<int> domino {1, 2};
    std::uniform_int_distribution<int> mino {1, 1};
    std::cout << h << " " << w << "\n";
    while(sum > 0) {
        int index = 0;
        if(sum == 1) {
            index = mino(urbg);
        }
        else if(sum == 2) {
            index = domino(urbg);
        }
        else if(sum == 3) {
            index = trimino(urbg);
        }
        else if(sum == 4) {
            index = tetramino(urbg);
        }
        else {
            index = pentamino(urbg);
        }
        std::cout << index_to_pieces[index] << "\n";
        if(index < 2) {
            sum -= 1;
        }
        else if(index < 3) {
            sum -= 2;
        }
        else if(index < 5) {
            sum -= 3;
        }
        else if(index < 12) {
            sum -= 4;
        }
        else {
            sum -= 5;
        }
    }
}

int main(int argc, char** argv) {
    // function to print a data structure for the borders
    // print_borders();
    
    // function to visualize the borders outputted from print_borders
    // visual_borders();
    
    // given a width and height, output a random list of pieces that could potentially tile it
    int seed = 0;
    for (int i = 1; i < argc; i++) {
        // sets the file name to read the pieces from
        if (strcmp(argv[i], "--height") == 0) {
            height = std::atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--width") == 0) {
            width = std::atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "--seed") == 0) {
            seed = std::atoi(argv[i + 1]);
        }
    }
    randomize_board(height, width, seed);
    return 0;
}