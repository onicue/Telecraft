#pragma once
#include <sstream>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include "../utils/fix_string.hpp"
#include "../utils/TgTypes.hpp"

namespace telegram {
namespace core{

class BaseMethod;

struct BaseParameter {
  virtual ~BaseParameter() {}
  virtual void serialize(const std::string& value) = 0;
};

template<typename T>
T ConvertStringTo (const std::string &str) {
  std::istringstream ss(str);
  T return_var;
  ss >> return_var;
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

  ParametersBuilder() : _name(name.value) {}

  ParametersBuilder(T value) : _name(name.value), value(value) {}

  virtual T get () const { return this->value; }
  virtual void set (const T& value) { this->value = value; }

  virtual void serialize (const std::string& value) override {
    this->value = ConvertStringTo<T>(value);
  };

  std::string getName() const { return _name; }
protected:
  T value;
  std::string _name;
};

template<TgType... Args>
class ParametersContainer{
public:
  ParametersContainer(){
    generateParameters<Args...>();
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
    return parameters_map.contains(name);
  }

  auto& at(std::string& name){
    return parameters_map.at(name);
  }
private:
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
  void generateParameters() {
      ((parameters_map.emplace(Ts().getName(), &std::get<Ts>(parameters))), ...);
  }

  std::unordered_map<std::string, BaseParameter*> parameters_map;


  std::tuple<Args...> parameters;
};

} //core
} //telegram
