//
// Created by wwz on 2025/9/27.
//

#include "FNN.h"
#include "utils.h"

FNN::FNN()  : rng(getRng()), l1(), l2(l1, rng), l3(l2, rng), l4(l3, rng) {
    input.alloc();
    grad_input.alloc();
    l1.grad_input = grad_input;
    l1.input = input;
    l1.init(rng);
}

Vector<double, 4> FNN::forward() {
    l1.forward(false);
    l2.forward(false);
    l3.forward(false);
    l4.forward(true);
    return l4.getOutput();
}

void FNN::backward(const Vector<double, 4>& grad_output) {
    l4.setGradOutput(grad_output);
    l4.backward(learning_rate, true);
    l3.backward(learning_rate, false);
    l2.backward(learning_rate, false);
    l1.backward(learning_rate, false);
}

void FNN::copy(const FNN &nn) {
    l1.copy(nn.l1);
    l2.copy(nn.l2);
    l3.copy(nn.l3);
    l4.copy(nn.l4);
    learning_rate = nn.learning_rate;
}

void FNN::setLearningRate(const double rate) {
    learning_rate = rate;
}

void FNN::setInput(const int *board) {
    for (int i = 0; i < 16; i++) {
        input[i] = board[i];
    }
}
