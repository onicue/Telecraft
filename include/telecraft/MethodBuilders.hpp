#pragma once
#include "core/HttpManagement.hpp"
#include "core/ParamsManagement.hpp"
#include <string>

namespace telegram {
namespace core {

class MethodInfo {
  public:
    MethodInfo(const std::string& name, http::ContentType content_type, http::Method method)
        : name(name), contentType(content_type), method(method) {}

    std::string getName(){ return name; }
    http::ContentType getContentType(){ return contentType; }
    http::Method getMethod(){ return method; }

  private:
    http::ContentType contentType;
    http::Method method;
    std::string name;
};

class MethodBuilder : public MethodInfo {
  public:
    virtual ~MethodBuilder(){};
    MethodBuilder(const std::string& name) : MethodInfo(name, http::ContentType::Json, http::Method::POST) {}
    MethodBuilder(const std::string& name, http::ContentType content_type) : MethodInfo(name, content_type, http::Method::POST) {}
    MethodBuilder(const std::string& name, http::ContentType content_type, http::Method method) : MethodInfo(name, content_type, method) {}

    virtual void build() = 0;
    /* virtual void serializeMessageToJson(const string& http) = 0; */
    virtual std::string getBody() = 0;
};

template<TgTypes... Args>
class MethodUI {
  public:
    virtual ~MethodUI(){}

    template<TgTypes ParamType, typename ValueType = ParamType::ValueType>
    constexpr ValueType get() {
      if (parameters.template contains<ParamType>()) {
        return parameters.template at<ParamType>().get();
      } else {
        throw std::invalid_argument("Invalid argument in method get");
      }
    }

    template<TgTypes ParamType, typename ValueType = ParamType::ValueType>
    constexpr void set(const ValueType& value) {
      if (parameters.template contains<ParamType>()) {
        parameters.template at<ParamType>().set(value);
      } else {
        throw std::invalid_argument("Invalid argument in method get");
      }
    }

    template<TgTypes ParamType>
    constexpr std::string getParameterName() {
      return parameters.template at<ParamType>().name;
    }

  protected:
    ParametersContainer<Args...> parameters;
};

} //core
} //telegram
