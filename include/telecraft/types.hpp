#pragma once
#include "types_core.hpp"
#include <string>
namespace telegram {
namespace types {

#define CLASS_NAME(Class) #Class

#define DEFINE_PARAMETERS_BUILDER(Class, Type)\
        struct Class : public core::ParametersBuilder<Type> { \
          Class() : core::ParametersBuilder<Type>(CLASS_NAME(Class)) {}\
        };

struct chat_id : public core::ParametersBuilder<int> {
  chat_id() : core::ParametersBuilder<ValueType>("chat_id") {}
  void set(const std::string& value) {
    value_ = std::stoi(value);
  }

  void set(const int& value) {
    value_ = value;
  }
};

DEFINE_PARAMETERS_BUILDER(text, std::string);

} // namespace types
} //namespace telegram
