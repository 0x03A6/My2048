//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_LAYER_H
#define MY2048_LAYER_H

#include <algorithm>
#include <random>

#include "utils.h"
#include "Vector.h"

enum class Activation { Sigmoid, ReLU, Linear };

template<int LastSize, int Size>
class Layer {
    Matrix<double, Size, LastSize> weights;
    Vector<double, Size> bias;
    Vector<double, Size> delta;
    Vector<double, LastSize> input;
    Vector<double, Size> output;
    Vector<double, LastSize> grad_input;
    Vector<double, Size> grad_output;
    static constexpr double InitLimitSigmoid = std::sqrt(6.0 / (LastSize  + Size));
    static constexpr double InitLimitReLU = std::sqrt(6.0 / LastSize);
    Activation activation = Activation::Sigmoid;

    template<int A, int B>
    friend class Layer;

    friend class FNN;

    public:

    // 防止和链接不同层的语义混淆。
    Layer(const Layer &) = delete;

    Layer() {
        weights.alloc();
        bias.alloc();
        delta.alloc();
        output.alloc();
        grad_output.alloc();
    }

    template<int PrevSize>
    explicit Layer(Layer<PrevSize, LastSize> &from) {
        weights.alloc();
        bias.alloc();
        delta.alloc();
        output.alloc();
        grad_output.alloc();
        input = from.output;
        grad_input = from.grad_output;
    }

    template<int PrevSize>
    Layer(Layer<PrevSize, LastSize> &from, std::mt19937 &rng) : Layer(from) {
        init(rng);
    }

    Layer(Vector<double, LastSize> input_, Vector<double, LastSize> grad_input_)
        : input(input_), grad_input(grad_input_) {
        weights.alloc();
        bias.alloc();
        delta.alloc();
        output.alloc();
        grad_output.alloc();
    }

    Layer(Vector<double, LastSize> input_, Vector<double, LastSize> grad_input_, std::mt19937 &rng) : Layer(input_, grad_input_) {
        init(rng);
    }

    void init(std::mt19937 &rng) {
        std::uniform_real_distribution<double> dis(
            activation == Activation::Sigmoid ? -InitLimitSigmoid : -InitLimitReLU,
            activation == Activation::Sigmoid ? InitLimitSigmoid : InitLimitReLU);
        for (int i = 0; i < Size; i++) {
            for (int j = 0; j < LastSize; j++) {
                weights[i][j] = dis(rng);
            }
            bias[i] = dis(rng);
        }
    }
    void forward() {
        output.fromMultiply(weights, input);
        output += bias;
        switch (activation) {
            case Activation::Sigmoid:
                for (int i = 0; i < Size; i++) {
                    output[i] = sigmoid(output[i]);
                }
                break;
            case Activation::ReLU:
                for (int i = 0; i < Size; i++) {
                    output[i] = ReLU(output[i]);
                }
                break;
            // case Activation::Linear:
            //     for (int i = 0; i < Size; i++) {
            //         output[i] = linear(output[i]);
            //     }
        }
    }
    void backward(const double learning_rate) {
        switch (activation) {
            case Activation::Sigmoid:
                for (int i = 0; i < Size; i++) {
                    delta[i] = grad_output[i] * output[i] * (1.0 - output[i]);
                }
                break;
            case Activation::ReLU:
                for (int i = 0; i < Size; i++) {
                    delta[i] = grad_output[i] * (output[i] > 0.0 ? 1.0 : 0.0);
                }
                break;
            case Activation::Linear:
                memcpy(delta.getData(), grad_output.getData(), Size * sizeof(double));
                // for (int i = 0; i < Size; i++) {
                //     delta[i] = grad_output[i];
                // }
        }
        for (int i = 0; i < Size; i++) {
            for (int j = 0; j < LastSize; j++) {
                const double dw_ij = delta[i] * input[j];
                weights[i][j] -= learning_rate * dw_ij;
            }
            bias[i] -= learning_rate * delta[i];
        }
        grad_input.clear();
        for (int j = 0; j < LastSize; j++) {
            for (int i = 0; i < Size; i++) {
                grad_input[j] += weights[i][j] * delta[i];
            }
        }
    }
    Vector<double, Size> getOutput() {
        return (Vector<double, Size>){ output };
    }
    void setGradOutput(const Vector<double, Size> grad_output_) {
        grad_output = grad_output_;
    }
    void copy(const Layer &other) {
        weights.copy(other.weights);
        bias.copy(other.bias);
    }
    void setActivation(const Activation activation_) {
        activation = activation_;
    }
};


#endif //MY2048_LAYER_H