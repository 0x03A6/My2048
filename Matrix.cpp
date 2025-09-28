//
// Created by wwz on 2025/9/28.
//

#include "Matrix.h"

#include <cstring>

template<typename T, int N, int M>
Matrix<T, N, M> Matrix<T, N, M>::operator+(const Matrix &other) const {
    static Matrix result;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

template<typename T, int N, int M>
Matrix<T, N, M> Matrix<T, N, M>::operator-(const Matrix &other) const {
    static Matrix result;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

template<typename T, int N, int M>
template<int P>
Matrix<T, N, P> Matrix<T, N, M>::operator*(const Matrix<T, M, P> &other) const {
    static Matrix<T, N, P> result;
    result.clear();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            const T temp = data[i][k];
            for (int j = 0; j < P; j++) {
                result.data[i][j] += temp * other.data[k][j];
            }
        }
    }
    return result;
}

template<typename T, int N, int M>
Matrix<T, N, M> &Matrix<T, N, M>::operator+=(const Matrix &other) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}

template<typename T, int N, int M>
Matrix<T, N, M> & Matrix<T, N, M>::operator-=(const Matrix &other) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            data[i][j] -= other.data[i][j];
        }
    }
    return *this;
}

template<typename T, int N, int M>
template<int P>
void Matrix<T, N, M>::fromMultiply(const Matrix<T, N, P> &a, const Matrix<T, P, M> &b) {
    clear();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            const T temp = a.data[i][k];
            for (int j = 0; j < P; j++) {
                data[i][j] += temp * b.data[k][j];
            }
        }
    }
}

template<typename T, int N, int M>
void Matrix<T, N, M>::clear() {
    memset(data, 0, sizeof(data));
}

template<typename T, int N, int M>
T * Matrix<T, N, M>::operator[](const int i) const {
    return data[i];
}

template<typename T, int N, int M>
Matrix<T, M, N> Matrix<T, N, M>::transpose() const {
    static Matrix<T, M, N> result;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result.data[j][i] = data[i][j];
        }
    }
    return result;
}
