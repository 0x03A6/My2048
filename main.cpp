#include <iostream>

#include <conio.h>

#include "Game2048.h"
#include "utils.h"
#include "Matrix.h"

int main() {
    Matrix<int, 3, 3> mat;
    mat.clear();
    std::random_device rd;
    std::mt19937 rng(rd());
    Game2048 game(rng);
    game.play(rng);
    return 0;
}