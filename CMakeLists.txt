cmake_minimum_required(VERSION 4.0)
project(My2048)

set(CMAKE_CXX_STANDARD 20)

add_executable(My2048 main.cpp
        Game2048.cpp
        Game2048.h
        FNN.cpp
        FNN.h
        print.h
        utils.cpp
        utils.h
        Matrix.cpp
        Matrix.h)
