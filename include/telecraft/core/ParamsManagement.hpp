#pragma once
#include <string>
#include <sstream>
#include <tuple>
#include <type_traits>
#include "../utils/fix_string.hpp"

namespace telegram {
namespace core{

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

template<typename T, StringLiteral set_name>
struct ParametersBuilder : BaseParameter {
  using ValueType = T;

  static_assert(!std::is_same_v<T, void>, "Warning: ParametersBuilder cannot be instantiated with void type");

  ParametersBuilder() : name(set_name.value) {}

  ParametersBuilder(T value) : name(set_name.value), value(value) {}

  virtual T get () { return this->value; }
  virtual void set (const T& value) { this->value = value; }

  virtual void serialize (const std::string& value) override {
    this->value = ConvertStringTo<T>(value);
  };

  const std::string name;

protected:
  T value;
};

template<typename T>
concept TgTypeEntity = std::is_base_of_v<BaseParameter, T> &&
    requires (T obj) {
      { obj.get() } -> std::convertible_to<typename T::ValueType>;
      { obj.set(std::declval<decltype(obj.get())>()) };
      { obj.name };
    };

template<TgTypeEntity... Args>
class ParametersContainer{
public:
  ParametersContainer(){}

  template<TgTypeEntity T>
  auto& at(){
    return std::get<T>(parameters);
  }

  template <typename Func>
  void forEach(Func&& func) {
    std::apply([&func](auto&... args) { (func(args), ...); }, parameters);
  }

  template<TgTypeEntity T>
  static constexpr bool contains() {
    return containsImpl<T, Args...>();
  }

private:
  template<TgTypeEntity T, TgTypeEntity First, TgTypeEntity... Rest>
  static constexpr bool containsImpl() {
    if constexpr (std::is_same_v<T, First>) {
      return true;
    } else if constexpr (sizeof...(Rest) > 0) {
      return containsImpl<T, Rest...>();
    } else {
      return false;
    }
  }

  std::tuple<Args...> parameters;
};

} //core

template<typename T>
concept TgTypes = telegram::core::TgTypeEntity<T>;

} //telegram
