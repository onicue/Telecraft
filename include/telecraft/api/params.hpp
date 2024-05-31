#pragma once
#include "../core/ParamsManagement.hpp"
#include <string>
#include <iostream>

namespace telegram {
namespace param {

#define CLASS_NAME(Class) #Class

#define DEFINE_PARAMETERS_BUILDER(Class, Type)\
        struct Class : public core::ParametersBuilder<Type> { \
          Class(Type value) : ParametersBuilder(CLASS_NAME(Class)){\
            this->value_ = value;\
          }\
          Class() : ParametersBuilder(CLASS_NAME(Class)) {}\
        };

struct chat_id : public core::ParametersBuilder<long int, "chat_id"> {
  chat_id() {}
  chat_id(auto value) {
    this->set(value);
  }
  void set(const std::string& value) {
    try {
      value_ = std::stol(value);
    } catch (const std::invalid_argument& e) {
      std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
      std::cerr << "Out of range: " << e.what() << std::endl;
    }
  }

  void set(const long int& value) {
    value_ = value;
  }
};

DEFINE_PARAMETERS_BUILDER(text, std::string);

} //param
} //telegram
