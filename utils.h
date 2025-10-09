//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_UTILS_H
#define MY2048_UTILS_H

#include <random>

template<typename T>
T sigmoid(const T x) {
    return static_cast<T>(1) / (static_cast<T>(1) + exp(-x));
}

template<typename T>
T sigmoidDerivative(T x) {
    const T temp = sigmoid(x);
    return temp * (static_cast<T>(1) - temp);
}

template<typename T>
T ReLU(const T x) {
    return x > 0 ? x : 0;
}

template<typename T>
T ReLUDerivative(const T x) {
    return x > 0 ? 1 : 0;
}

template<typename T>
T linear(const T x) {
    return x;
}

template<typename T>
T linearDerivative(const T x) {
    return 1;
}

inline std::mt19937 &getRng() {
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

#endif //MY2048_UTILS_H