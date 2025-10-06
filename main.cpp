#include <iostream>

#include <conio.h>

#include "Game2048.h"
#include "Layer.h"

int main() {
    Matrix<int, 3, 3> mat = {};
    mat.clear();
    std::random_device rd;
    std::mt19937 rng(rd());
    Game2048 game(rng);
    game.play(rng);

    Vector<double, 256> v;

    Layer<256, 256> layer;
    layer.forward(v);
    return 0;
}