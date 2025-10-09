//
// Created by wwz on 2025/9/27.
//

#ifndef MY2048_FNN_H
#define MY2048_FNN_H

#include <random>

#include "hyperparams.h"
#include "Vector.h"
#include "Layer.h"
#include "ReplayBuffer.h"

class FNN {
    Vector<double, 16> input;
    Vector<double, 16> grad_input;
    Layer<16, 128> l1;
    Layer<128, 64> l2;
    Layer<64, 32> l3;
    Layer<32, 4> l4;
    double learning_rate = InitLearningRate;

public:
    explicit FNN();
    Vector<double, 4> forward();
    void backward(const Vector<double, 4>& grad_output);
    void copy(const FNN &nn);
    void setLearningRate(double rate);
    void setInput(const int *board);
};


#endif //MY2048_FNN_H