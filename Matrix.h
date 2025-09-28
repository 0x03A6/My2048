//
// Created by wwz on 2025/9/28.
//

#ifndef MY2048_MATRIX_H
#define MY2048_MATRIX_H

template<typename T, int N, int M>
class Matrix {
    T vals[N][M];
public:
    Matrix<T, N, M> operator+(const Matrix<T, N, M>& other) const;
};

template<typename T, int N, int M>
Matrix<T, N, M> Matrix<T, N, M>::operator+(const Matrix<T, N, M> &other) const {
    Matrix<T, N, M> result;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result.vals[i][j] = vals[i][j] + other.vals[i][j];
        }
    }
    return result;
}


#endif //MY2048_MATRIX_H
