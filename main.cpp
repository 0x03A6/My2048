#include <iostream>

#include <conio.h>

#include "Game2048.h"
#include "Layer.h"
#include "FNN.h"

int main() {

    // Matrix<double, 3, 3> mat({ 1, 0, 1, 0, 2, 0, 0, 0, 1});
    // mat.print();
    // Vector<double, 3> vec({ 1, 5, 1 });
    // vec.print();
    // Vector<double, 3> r;
    // r.alloc();
    // r.fromMultiply(mat, vec);
    // r += { 2, 3, 4 };
    // r.print();

    Vector<double, 16> input;
    input.alloc();
    Vector<double, 16> grad_input;
    grad_input.alloc();
    Layer<16, 16> l1(input, grad_input);
    Layer<16, 16> l2(l1);
    l2.forward();

    FNN nn(1024);

    // std::random_device rd;
    // std::mt19937 rng(rd());
    // Game2048 game(rng);
    // game.play(rng);

    return 0;
}