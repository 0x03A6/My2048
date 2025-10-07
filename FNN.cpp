//
// Created by wwz on 2025/9/27.
//

#include "FNN.h"
#include "utils.h"

FNN::FNN(const unsigned int seed)  : rng(seed), l1(input, grad_input, rng), l2(l1, rng), l3(l2, rng), l4(l3, rng) {
    input.alloc();
    grad_input.alloc();
}
