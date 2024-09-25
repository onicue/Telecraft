#pragma once
#include "../core/ParamsManagement.hpp"
#include <string>
#include <iostream>

namespace telegram {
namespace param {

#define CLASS_NAME(Class) #Class

#define DEFINE_TG_PARAMETER(Class, Type)\
        struct Class : public core::ParametersBuilder<Type, CLASS_NAME(Class)> { \
          using ParametersBuilder::ParametersBuilder; \
        };

struct chat_id : public core::ParametersBuilder<long int, "chat_id"> {
  chat_id() {}
  chat_id(auto value) {
    this->set(value);
  }
  void set(const std::string& value) {
    try {
      this->value = std::stol(value);
    } catch (const std::invalid_argument& e) {
      std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
      std::cerr << "Out of range: " << e.what() << std::endl;
    }
  }

  void set(const long int& value) {
    this->value = value;
  }
};

DEFINE_TG_PARAMETER(text, std::string);

} //param
} //telegram
