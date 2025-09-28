//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_MATRIX_H
#define MY2048_MATRIX_H

template<typename T, int N, int M>
class Matrix {
    T data[N][M];
public:
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    template<int P>
    Matrix<T, N, P> operator*(const Matrix<T, M, P>& other) const;
    Matrix &operator+=(const Matrix& other);
    Matrix &operator-=(const Matrix& other);
    template<int P>
    void fromMultiply(const Matrix<T, N, P> &a, const Matrix<T, P, M> &b);
    void clear();
    T *operator[](int i) const;
    Matrix<T, M, N> transpose() const;
};


#endif //MY2048_MATRIX_H
