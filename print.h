//
// Created by wwz on 2025/9/27.
//

#ifndef MY2048_PRINT_H
#define MY2048_PRINT_H

#include <iostream>
#include <tuple>
#include <string_view>
#include <array>

template<typename T, typename... Args>
consteval auto parseStr(const std::string_view &str) {
    for (auto it = str.begin(); (it + 1) != str.end(); ++it) {
        if (*it == '{' && *(it + 1) == '}') {
            const std::string_view pre(str.begin(), it);
            const std::string_view rem(it + 2, str.end());
            if constexpr (sizeof...(Args) == 0)
                return std::make_tuple(pre, T(), rem);
            else
                return std::tuple_cat(std::make_tuple(pre, T()), parseStr<Args...>(rem));
        }
    }
}

template<typename T, typename... Args>
consteval auto parseStrWithInit(const std::string_view &str, T x, Args... args) {
    for (auto it = str.begin(); (it + 1) != str.end(); ++it) {
        if (*it == '{' && *(it + 1) == '}') {
            const std::string_view pre(str.begin(), it);
            const std::string_view rem(it + 2, str.end());
            if constexpr (sizeof...(Args) == 0)
                return std::make_tuple(pre, x, rem);
            else
                return std::tuple_cat(std::make_tuple(pre, x), parseStrWithInit(rem, args...));
        }
    }
}

template<typename... Args>
void printPack(Args... args) {
    (std::cout << ... << args);
    // another impl: ((std::cout << args), ...);
}

template<typename... Args>
void printTuple(std::tuple<Args...> t) {
    std::apply(printPack<Args...>, t);
}

template<int NowId = 0, typename TupleType, typename FirstArg, typename... Args>
void fillValue(TupleType &t, FirstArg arg, Args... args) {
    if constexpr (std::is_same_v<typeof(std::get<NowId>(t)), FirstArg>) {
        std::get<NowId>(t) = arg;
        if constexpr (sizeof...(Args) > 0) {
            fillValue<NowId + 1, TupleType, Args...>(t, args...);
        }
    } else {
        fillValue<NowId + 1, TupleType, FirstArg, Args...>(t, arg, args...);
    }
}

#endif //MY2048_PRINT_H