/// @todo 可以把它改成用 shared_ptr 维护的。

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
    bool owned;
public:
    Vector() {
        data = nullptr;
        owned = false;
    }
    Vector(const Vector &v) {
        data = v.data;
        owned = false;
    }
    Vector(Vector &&) noexcept = default;
    Vector(const std::initializer_list<double> src) {
        if (src.size() != Size) {
            data = nullptr;
            owned = false;
            return;
        }
        data = new T[Size];
        owned = true;
        memcpy(data, src.begin(), Size * sizeof(T));
    }
    // Vector &operator=(const Vector &) = default;
    // Vector &operator=(Vector &&) noexcept = default;
    ~Vector() {
        if (owned)
            delete[] data;
    }
    void alloc() {
        data = new T[Size];
        owned = true;
    }
    Vector &operator=(const Vector v) {
        if (this != &v) {
            if (owned) {
                delete[] data;
                owned = false;
            }
            data = v.data;
        }
        return *this;
    }
    T &operator[](const int index) const {
        return data[index];
    }
    // 大粪设计。让它在析构的时候不要把内存释放。记得在计算之后让存结果的对象认领 data 内存。
    void move() {
        owned = false;
    }
    // 这个可以认领 data 内存，析构的时候释放它。
    void claimOwned() {
        owned = true;
    }
    Vector operator+(const Vector v) const {
        Vector r;
        r.alloc();
        for (int i = 0; i < Size; i++) {
            r[i] = data[i] + v[i];
        }
        r.move();
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
        r.alloc();
        for (int i = 0; i < Size; i++) {
            r[i] = data[i] - v[i];
        }
        r.move();
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
    template<int N>
    void fromMultiply(const Matrix<T, Size, N> mat, const Vector<T, N> v) {
        clear();
        for (int j = 0; j < N; j++) {
            for (int i = 0; i < Size; i++) {
                data[i] += mat[i][j] * v[j];
            }
        }
    }
    void print() const {
        if (data == nullptr) {
            std::cout << "(Vector NULL)\n";
            return;
        }
        std::cout << "(Vector [ ";
        for (int i = 0; i < Size; i++) {
            std::cout << data[i] << ' ';
        }
        std::cout << "] ^ T)\n";
    }

};

template<typename T, int N, int M>
Vector<T, N> operator*(const Matrix<T, N, M> mat, const Vector<T, M> &v) {
    Vector<T, N> r;
    r.alloc();
    r.clear();
    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
            r[j] += mat[i][j] * v[j];
        }
    }
    r.move();
    return r;
}

#endif //MY2048_VECTOR_H