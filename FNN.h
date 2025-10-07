//
// Created by wwz on 2025/9/27.
//

#ifndef MY2048_FNN_H
#define MY2048_FNN_H

#include <random>

#include "Vector.h"
#include "Layer.h"

class FNN {
    Vector<double, 16> input;
    Vector<double, 16> grad_input;
    Layer<16, 256> l1;
    Layer<256, 128> l2;
    Layer<128, 64> l3;
    Layer<64, 4> l4;
    std::mt19937 rng;

public:
    FNN(unsigned int seed);
    Vector<double, 4> forward();
};


#endif //MY2048_FNN_H