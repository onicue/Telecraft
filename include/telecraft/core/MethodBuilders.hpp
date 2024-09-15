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
};

template<StringLiteral name,
         http::ContentType content_type = http::ContentType::Json,
         http::Method method = http::Method::POST>
class MethodInfo : public BaseMethod {
public:
  MethodInfo() : _name(name), _method(method), _content_type(content_type) {}
  std::string getName() override { return _name; }
  http::ContentType getContentType() override { return _content_type; }
  http::Method getMethod() override { return _method; }
private:
  http::ContentType _content_type;
  http::Method _method;
  std::string _name;
};

template<TgType... AvailableTypes>
class ParamManager {
public:
  virtual ~ParamManager() {}

  template<TgType ParamType, typename ValueType = typename ParamType::ValueType>
  constexpr ValueType get() {
    if (checkContent<ParamType>("get")) {
      return parameters.template at<ParamType>().get();
    }
  }

  template<TgType ParamType, typename ValueType = typename ParamType::ValueType>
  constexpr void set(const ValueType& value) {
    if (checkContent<ParamType>("set")) {
      parameters.template at<ParamType>().set(value);
    }
  }

  template<TgType ParamType>
  constexpr std::string getParameterName() {
    return parameters.template at<ParamType>().getName();
  }

  template<TgType ParamType>
  constexpr void setParam(ParamType& param) {
    if (checkContent<ParamType>("setParam")) {
      parameters.template at<ParamType>().set(param.get());
    }
  }

  template <TgType... Params>
  constexpr void setParams(Params&... params) {
    (setParam(params), ...);
  }

  template<TgType... T>
  friend std::string json::serialize(core::ParamManager<T...>& mngr);
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
