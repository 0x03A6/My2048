//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_LAYER_H
#define MY2048_LAYER_H

#include "Matrix.h"

template<int LastSize, int Size>
class Layer {
    Matrix<double, LastSize, Size> weights;
};


#endif //MY2048_LAYER_H