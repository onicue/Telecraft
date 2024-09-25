#pragma once
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include "../utils/fix_string.hpp"
#include "../utils/TgTypes.hpp"
#include "../utils/json.hpp"

namespace telegram {
namespace core{

class BaseMethod;

struct BaseParameter {
  virtual ~BaseParameter() {}
  virtual void deserialize(const glz::json_t& value) = 0;
  virtual void deserialize(const std::string& value) = 0;
  virtual constexpr std::string getName() const = 0;
};

template<typename T>
T ConvertStringTo (const std::string &str) {
  std::istringstream ss(str);
  T return_var;
  if (!(ss >> return_var)) {
    throw std::invalid_argument("Invalid conversion from string to target type.");
  }
  if (ss.fail() || !ss.eof()) {
    throw std::invalid_argument("Conversion resulted in leftover input.");
  }
  return return_var;
}

template<>
std::string ConvertStringTo <std::string>(const std::string &str) {
  return str;
}

template<typename T, StringLiteral name>
struct ParametersBuilder : BaseParameter {
  using ValueType = T;

  static_assert(!std::is_same_v<T, void>, "Warning: ParametersBuilder cannot be instantiated with void type");

  ParametersBuilder() {}

  ParametersBuilder(T value) : value(value) {}

  virtual T get () const {
    return this->value;
  }

  virtual void set (const T& value) {
    this->value = value;
  }

  constexpr std::string getName() const override {
    return name;
  }

  virtual void deserialize (const glz::json_t& value) override {
    using ParamType =  typename json::TypeMapper<T>::type;
    this->value = static_cast<T>(value.get<ParamType>());
  }

  virtual void deserialize(const std::string& value) override {
    try {
      this->value = ConvertStringTo<T>(value);
    } catch (const std::invalid_argument& e) {
      std::cerr << "Cannot convert string for " << name << ". Error: " << e.what() << std::endl;
    }
  }
protected:
  T value;
};

template<TgType... Args>
class ParametersContainer{
public:
  ParametersContainer(){
    defineIndexMap<Args...>();
  }

  template<TgType T>
  auto& at(){
    return std::get<T>(parameters);
  }

  template <typename Func>
  void forEach(Func&& func) {
    std::apply([&func](auto&... args) { (func(args), ...); }, parameters);
  }

  template<TgType T>
  static constexpr bool contains() {
    return contains<T, Args...>();
  }

  template <typename T>
  constexpr bool isMethod(T&& param) const {
    using ParamType = std::decay_t<T>;
    return std::is_base_of_v<BaseMethod, ParamType>;
  }

  bool contains(const std::string& name) {
    return paramsMap.find(name) != paramsMap.end();
  }

  auto& at(const std::string& name){
    return *(paramsMap.at(name));
  }
private:
  using ParamVariant = BaseParameter*;

  template<TgType T, TgType First, TgType... Rest>
  static constexpr bool contains() {
    if constexpr (std::is_same_v<T, First>) {
      return true;
    } else if constexpr (sizeof...(Rest) > 0) {
      return contains<T, Rest...>();
    } else {
      return false;
    }
  }

  template<typename... Ts>
  void defineIndexMap() {
    (paramsMap.emplace(Ts().getName(), static_cast<ParamVariant>(&ParametersContainer::at<Ts>())), ...);
  }

  auto& checkMapAndGetVal(const std::string& name) {
    auto it = paramsMap.find(name);
    if (it == paramsMap.end()) {
        throw std::runtime_error("Parameter " + std::string(name) + " not found");
    }
    return it->second;
  }

  std::unordered_map<std::string, ParamVariant> paramsMap;
  std::tuple<Args...> parameters;
};

} //core
} //telegram
