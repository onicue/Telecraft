#pragma once
#include "../core/params_mgmt.hpp"
#include <string>

namespace telegram {
namespace param {

#define CLASS_NAME(Class) #Class

#define DEFINE_PARAMETERS_BUILDER(Class, Type)\
        struct Class : public core::ParametersBuilder<Type> { \
          Class() : core::ParametersBuilder<Type>(CLASS_NAME(Class)) {}\
        };

struct chat_id : public core::ParametersBuilder<long int> {
  chat_id() : core::ParametersBuilder<ValueType>("chat_id") {}
  void set(const std::string& value) {
    value_ = std::stol(value);
  }

  void set(const long int& value) {
    value_ = value;
  }
};

DEFINE_PARAMETERS_BUILDER(text, std::string);

} //param
} //telegram
