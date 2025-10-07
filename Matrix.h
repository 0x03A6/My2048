/// @todo 可以把它改成用 shared_ptr 维护的。

//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_MATRIX_H
#define MY2048_MATRIX_H

#include <cstring>
#include <iostream>

/// @attention 拷贝构造函数和移动构造函数是浅拷贝，对于数据来说可以理解为语义是引用，不会创建新数据，而是绑定在老数据上。
/// @attention 赋值运算符只有一种语义就是浅拷贝，同上，对于数据来说可以理解为语义是引用。
/// @attention 提供了 clear 和 copy 方法替代数据层面的拷贝语义。
/// @bug 没遵守 rules of five 真是抱歉了
template<typename T, int N, int M>
class Matrix {
    T *data;
    bool owned;
public:
	Matrix() {
	    data = nullptr;
	    owned = false;
	}
    Matrix(const Matrix &v) {
	    data = v.data;
	    owned = false;
	}
    Matrix(Matrix &&) noexcept = default;
    Matrix(const std::initializer_list<double> src) {
        if (src.size() != N * M) {
            data = nullptr;
            owned = false;
            return;
        }
        data = new T[N * M];
        owned = true;
        memcpy(data, src.begin(), N * M * sizeof(T));
    }
    // Matrix &operator=(const Matrix &) = default;
    // Matrix &operator=(Matrix &&) noexcept = default;
    ~Matrix() {
        if (owned)
            delete[] data;
    }
    Matrix &operator=(const Matrix mat) {
        if (this != &mat) {
            if (owned) {
                delete[] data;
                owned = false;
            }
            data = mat.data;
        }
        return *this;
    }
    void alloc();
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
    void print() const;
    void move();
    void claimOwned();
};

template<typename T, int N, int M>
void Matrix<T, N, M>::alloc() {
    data = new T[N * M];
    owned = true;
}

template<typename T, int N, int M>
T & Matrix<T, N, M>::at(const int i, const int j) const {
    return data[i * M + j];
}

template<typename T, int N, int M>
Matrix<T, N, M> Matrix<T, N, M>::operator+(const Matrix other) const {
    Matrix result;
    result.alloc();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result[i][j] = at(i, j) + other[i][j];
        }
    }
    result.move();
    return result;
}

template<typename T, int N, int M>
Matrix<T, N, M> Matrix<T, N, M>::operator-(const Matrix other) const {
    Matrix result;
    result.alloc();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result[i][j] = at(i, j) - other[i][j];
        }
    }
    result.move();
    return result;
}

template<typename T, int N, int M>
template<int P>
Matrix<T, N, P> Matrix<T, N, M>::operator*(const Matrix<T, M, P> &other) const {
    Matrix<T, N, P> result;
    result.alloc();
    result.clear();
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < M; k++) {
            const T temp = at(i, k);
            for (int j = 0; j < P; j++) {
                result[i][j] += temp * other[k][j];
            }
        }
    }
    result.move();
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
void Matrix<T, N, M>::print() const {
    if (data == nullptr) {
        std::cout << "(Matrix NULL)\n";
        return;
    }
    std::cout << "(Matrix [\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            std::cout << data[i * M + j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "])\n";
}

// 大粪设计。让它在析构的时候不要把内存释放。记得在计算之后让存结果的对象认领 data 内存。
template<typename T, int N, int M>
void Matrix<T, N, M>::move() {
    owned = false;
}

// 这个可以认领 data 内存，析构的时候释放它。
template<typename T, int N, int M>
void Matrix<T, N, M>::claimOwned() {
    owned = true;
}

#endif //MY2048_MATRIX_H
