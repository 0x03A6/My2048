//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_MATRIX_H
#define MY2048_MATRIX_H

#include <cstring>

/// @attention 拷贝构造函数和移动构造函数是浅拷贝，对于数据来说可以理解为语义是引用，不会创建新数据，而是绑定在老数据上。
/// @attention 赋值运算符只有一种语义就是浅拷贝，同上，对于数据来说可以理解为语义是引用。
/// @attention 提供了 clear 和 copy 方法替代数据层面的拷贝语义。
/// @bug 没遵守 rules of five 真是抱歉了
template<typename T, int N, int M>
class Matrix {
    T *data;
public:
	Matrix();
    Matrix(const Matrix &) = default;
    Matrix(Matrix &&) noexcept = default;
    // Matrix &operator=(const Matrix &) = default;
    // Matrix &operator=(Matrix &&) noexcept = default;
    Matrix &operator=(const Matrix mat) {
        if (this != &mat) {
            data = mat.data;
        }
        return *this;
    }
    T &at(int i, int j) const;
    Matrix operator+(Matrix other) const;
    Matrix operator-(Matrix other) const;
    template<int P>
    Matrix<T, N, P> operator*(const Matrix<T, M, P>& other) const;
    Matrix &operator+=(Matrix other);
    Matrix &operator-=(Matrix other);
    template<int P>
    void fromMultiply(Matrix<T, N, P> a, Matrix<T, P, M> b);
    void clear();
    T *operator[](int i) const;
    Matrix<T, M, N> transpose() const;
    void copy(Matrix mat);
    ~Matrix();
};

template<typename T, int N, int M>
Matrix<T, N, M>::Matrix() {
    data = new T[N * M];
}

template<typename T, int N, int M>
T & Matrix<T, N, M>::at(const int i, const int j) const {
    return data[i * M + j];
}

template<typename T, int N, int M>
Matrix<T, N, M> Matrix<T, N, M>::operator+(const Matrix other) const {
    static Matrix result;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result[i][j] = at(i, j) + other[i][j];
        }
    }
    return result;
}

template<typename T, int N, int M>
Matrix<T, N, M> Matrix<T, N, M>::operator-(const Matrix other) const {
    Matrix result;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result[i][j] = at(i, j) - other[i][j];
        }
    }
    return result;
}

template<typename T, int N, int M>
template<int P>
Matrix<T, N, P> Matrix<T, N, M>::operator*(const Matrix<T, M, P> &other) const {
    Matrix<T, N, P> result;
    result.clear();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            const T temp = at(i, k);
            for (int j = 0; j < P; j++) {
                result[i][j] += temp * other[k][j];
            }
        }
    }
    return result;
}

template<typename T, int N, int M>
Matrix<T, N, M> &Matrix<T, N, M>::operator+=(const Matrix other) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            at(i, j) += other[i][j];
        }
    }
    return *this;
}

template<typename T, int N, int M>
Matrix<T, N, M> & Matrix<T, N, M>::operator-=(const Matrix other) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            at(i, j) -= other[i][j];
        }
    }
    return *this;
}

template<typename T, int N, int M>
template<int P>
void Matrix<T, N, M>::fromMultiply(const Matrix<T, N, P> a, const Matrix<T, P, M> b) {
    clear();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            const T temp = a[i][k];
            for (int j = 0; j < P; j++) {
                at(i, j) += temp * b[k][j];
            }
        }
    }
}

template<typename T, int N, int M>
void Matrix<T, N, M>::clear() {
    memset(data, 0, sizeof(T) * N * M);
}

template<typename T, int N, int M>
T * Matrix<T, N, M>::operator[](const int i) const {
    return data + i * M;
}

template<typename T, int N, int M>
Matrix<T, M, N> Matrix<T, N, M>::transpose() const {
    static Matrix<T, M, N> result;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result[j][i] = at(i, j);
        }
    }
    return result;
}

template<typename T, int N, int M>
void Matrix<T, N, M>::copy(const Matrix mat) {
    memcpy(data, mat.data, sizeof(T) * N * M);
}

template<typename T, int N, int M>
Matrix<T, N, M>::~Matrix() {
    delete[] data;
}

#endif //MY2048_MATRIX_H
