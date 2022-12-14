#include "solver-stack.h"

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

int main(int argc, char** argv) {
    // print_borders();
    visual_borders();
    return 0;
}