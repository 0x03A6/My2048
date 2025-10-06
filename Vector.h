//
// Created by wwz on 2025/10/6.
//

#ifndef MY2048_VECTOR_H
#define MY2048_VECTOR_H

#include "Matrix.h"

/// @note 和 Matrix 的设计风格一致，这个作为单行、单列的矩阵的共同特化情况。
/// @attention 拷贝构造函数和移动构造函数是浅拷贝，对于数据来说可以理解为语义是引用，不会创建新数据，而是绑定在老数据上。
/// @attention 赋值运算符只有一种语义就是浅拷贝，同上，对于数据来说可以理解为语义是引用。
/// @attention 提供了 clear 和 copy 方法替代数据层面的拷贝语义。
/// @bug 没遵守 rules of five 真是抱歉了
template<typename T, int Size>
class Vector {
    T *data;
public:
    Vector() {
        data = new T[Size];
    }
    Vector(const Vector &) = default;
    Vector(Vector &&) noexcept = default;
    // Vector &operator=(const Vector &) = default;
    // Vector &operator=(Vector &&) noexcept = default;
    Vector &operator=(const Vector v) {
        if (this != &v)
            data = v.data;
        return *this;
    }
    ~Vector() {
        delete[] data;
    }
    T &operator[](const int index) const {
        return data[index];
    }
    Vector operator+(const Vector v) const {
        Vector r;
        for (int i = 0; i < Size; i++) {
            r[i] = data[i] + v[i];
        }
        return r;
    }
    Vector &operator+=(const Vector v) {
        for (int i = 0; i < Size; i++) {
            data[i] += + v[i];
        }
        return *this;
    }
    Vector operator-(const Vector v) const {
        Vector r;
        for (int i = 0; i < Size; i++) {
            r[i] = data[i] - v[i];
        }
        return r;
    }
    Vector &operator-=(const Vector v) {
        for (int i = 0; i < Size; i++) {
            data[i] -= v[i];
        }
        return *this;
    }
    void copy(const Vector v) {
        memcpy(data, v.data, sizeof(T) * Size);
    }
    void clear() {
        memset(data, 0, sizeof(T) * Size);
    }
};

template<typename T, int N, int M>
Vector<T, M> operator*(const Matrix<T, N, M> mat, const Vector<T, M> &v) {
    Vector<T, M> r;
    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
            r[j] += mat[i][j] * v[j];
        }
    }
    return r;
}

#endif //MY2048_VECTOR_H