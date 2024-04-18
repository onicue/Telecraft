#pragma once
#include <string>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <unordered_map>

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

template<typename T>
concept TgTypeEntity = std::is_base_of_v<BaseParameter, T> &&
    requires (T obj) {
      { obj.get() } -> std::convertible_to<typename T::ValueType>;
      { obj.set(std::declval<decltype(obj.get())>()) };
      { obj.name};
    };

template<typename T>
struct ParametersBuilder : BaseParameter {
using ValueType = T; //for type cast
    ParametersBuilder (const std::string& parameter_name) : name(parameter_name) {}
    virtual T get () { return value_; }
    virtual void set (const T& value) { value_ = value; }

    virtual void serialize (const std::string& value) override {
      value_ = ConvertStringTo<T>(value);
    };

    const std::string name;
  protected:
    T value_;
};

template<TgTypeEntity... Args>
class ParametersContainer{
  public:
    ParametersContainer(){
      generateParameters<Args...>();
    }

    template<TgTypeEntity T>
    auto& at(){
      return std::get<T>(parameters);
    }

    template <typename Func>
    void forEach(Func&& func) {
      std::apply([&func](auto&... args) { (func(args), ...); }, parameters);
    }

    template<TgTypeEntity T>
    bool contains() const {
      return parameters_map.contains(T().name);
    }

    bool contains(const std::string& name) const {
      return parameters_map.contains(name);
    }
  private:
    template<typename... Ts>
    void generateParameters() {
        ((parameters_map.emplace(Ts().name, &std::get<Ts>(parameters))), ...);
    }

    std::unordered_map<std::string, BaseParameter*> parameters_map;
    std::tuple<Args...> parameters;
};

} //core

template<typename T>
concept TgTypes = telegram::core::TgTypeEntity<T>;

} //telegram
