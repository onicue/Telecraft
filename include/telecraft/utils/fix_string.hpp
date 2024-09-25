#pragma once
#include <algorithm>
#include <string>
#include <iostream>

namespace telegram{

template<size_t N>
struct StringLiteral {
  char value[N];

  constexpr StringLiteral(const char (&str)[N]) {
    std::copy_n(str, N, value);
  }

  constexpr StringLiteral (const StringLiteral<N>& other){
    std::copy_n(other.value, N, value);
  }

  constexpr operator std::string() const {
    return std::string(value, N - 1);
  }

  friend std::ostream& operator<<(std::ostream& stream, const StringLiteral<N>& strLit) {
    stream << strLit.value;
    return stream;
  }
};

} //telegram
