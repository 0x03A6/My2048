//
// Created by wwz on 2025/9/28.
//

template<typename T>
T sigmoid(const T x) {
    return static_cast<T>(1) / (static_cast<T>(1) + exp(-x));
}

template<typename T>
T sigmoidDerivative(T x) {
    return sigmoid(x) / (static_cast<T>(1) - sigmoid(x));
}