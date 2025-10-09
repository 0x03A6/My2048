//
// Created by wwz on 2025/9/27.
//

#include "FNN.h"
#include "utils.h"

FNN::FNN()  : l1(), l2(l1), l3(l2), l4(l3) {
    input.alloc();
    grad_input.alloc();
    l1.grad_input = grad_input;
    l1.input = input;
    l1.setActivation(Activation::Sigmoid);
    l2.setActivation(Activation::ReLU);
    l3.setActivation(Activation::ReLU);
    l4.setActivation(Activation::Linear);
    l1.init(getRng());
    l2.init(getRng());
    l3.init(getRng());
    l4.init(getRng());
}

Vector<double, 4> FNN::forward() {
    l1.forward();
    l2.forward();
    l3.forward();
    l4.forward();
    return l4.getOutput();
}

void FNN::backward(const Vector<double, 4>& grad_output) {
    l4.setGradOutput(grad_output);
    l4.backward(learning_rate);
    l3.backward(learning_rate);
    l2.backward(learning_rate);
    l1.backward(learning_rate);
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
