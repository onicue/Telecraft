#pragma once
#include "HttpManagement.hpp"
#include "ParamsManagement.hpp"
#include "../utils/fix_string.hpp"
#include "../utils/json.hpp"
#include <string>

namespace telegram {
namespace core {

class BaseMethod {
public:
  virtual ~BaseMethod() {};
  BaseMethod() {}
  virtual void build() = 0;
  virtual std::string getBody() = 0;
  virtual std::string getName() = 0;
  virtual http::ContentType getContentType() = 0;
  virtual http::Method getMethod() = 0;
  virtual void deserialize(const std::string& json) = 0;
};

template<StringLiteral name,
         http::ContentType content_type = http::ContentType::Json,
         http::Method method = http::Method::POST>
class MethodInfo : public BaseMethod {
public:
  MethodInfo() {}
  constexpr std::string getName() override {
    return name;
  }

  constexpr http::ContentType getContentType() override {
    return content_type;
  }

  constexpr http::Method getMethod() override {
    return method;
  }
};

template<TgType... AvailableTypes>
class ParamManager {
public:
  virtual ~ParamManager() {}

  template<TgType ParamType, typename ValueType = typename ParamType::ValueType>
  ValueType get() {
    return parameters.template at<ParamType>().get();
  }

  template<TgType ParamType, typename ValueType = typename ParamType::ValueType>
  void set(const ValueType& value) {
    parameters.template at<ParamType>().set(value);
  }

  template<TgType ParamType>
  constexpr std::string getParameterName() {
    return parameters.template at<ParamType>().getName();
  }

  template<TgType ParamType>
  constexpr void setParam(ParamType& param) {
    parameters.template at<ParamType>().set(param.get());
  }

  template <TgType... Params>
  constexpr void setParams(Params&... params) {
    (setParam(params), ...);
  }

  template<TgType... T>
  friend std::string json::serialize(core::ParamManager<T...>& mngr);

  template<TgType... T>
  friend void json::deserialize(core::ParamManager<T...>& mngr, const std::string& json);
protected:
  template<TgType T>
  constexpr bool checkContent(std::string method_name = "") {
    std::string error_msg = "invalid argument";
    if (!method_name.empty()) {
      method_name = " " + method_name;
    }

    if constexpr (!parameters.template contains<T>()) {
      throw std::invalid_argument(error_msg + method_name);
      return false;
    }
    return true;
  }

  ParametersContainer<AvailableTypes...> parameters;
};

} // core
} // telegram
