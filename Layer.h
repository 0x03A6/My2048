//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_LAYER_H
#define MY2048_LAYER_H

#include "utils.h"
#include "Vector.h"

template<int LastSize, int Size>
class Layer {
    Matrix<double, LastSize, Size> weights;
    Vector<double, Size> bias;
    Vector<double, Size> output;
    public:
    Layer() = default;
    void forward(Vector<double, LastSize> input) {
        output = weights * input + bias;
        for (int i = 0; i < Size; i++) {
            output[i] = sigmoid(output[i]);
        }
    }
};


#endif //MY2048_LAYER_H