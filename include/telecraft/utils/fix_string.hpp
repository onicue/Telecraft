#pragma once
#include <algorithm>
#include <string>

template<size_t N>
struct StringLiteral {
    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N];

    constexpr operator std::string() const {
        return std::string(value, N - 1);
    }
};
