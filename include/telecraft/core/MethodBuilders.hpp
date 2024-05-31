#pragma once
#include "HttpManagement.hpp"
#include "ParamsManagement.hpp"
#include "../utils/fix_string.hpp"
#include <string>

namespace telegram {
namespace core {

class MethodInfo {
public:
  MethodInfo(const std::string& name, http::ContentType content_type, http::Method method)
      : _name(name), _content_type(content_type), _method(method) {}

  std::string getMethodName(){ return _name; }
  http::ContentType getContentType(){ return _content_type; }
  http::Method getMethod(){ return _method; }

private:
  http::ContentType _content_type;
  http::Method _method;
  std::string _name;
};

template<StringLiteral name,
          http::ContentType content_type = http::ContentType::Json,
          http::Method method = http::Method::POST>
class MethodBuilder : public MethodInfo {
public:
    virtual ~MethodBuilder(){};
    MethodBuilder() : MethodInfo(name.value, content_type, method){}
    virtual void build() = 0;
    /*virtual void serializeMessageToJson(const string& http) = 0; */
    virtual std::string getBody() = 0;
};

template<TgTypes... AvailableTypes>
class MethodUI {
public:
  virtual ~MethodUI(){}

  template<TgTypes ParamType, typename ValueType = ParamType::ValueType>
  constexpr ValueType get() {
    if (checkContent<ValueType>("get")) {
      return parameters.template at<ParamType>().get();
    }
  }

  template<TgTypes ParamType, typename ValueType = ParamType::ValueType>
  constexpr void set(const ValueType& value) {
    if (checkContent<ValueType>("set")) {
      parameters.template at<ParamType>().set(value);
    }
  }

  template<TgTypes ParamType>
  constexpr std::string getParameterName() {
    return parameters.template at<ParamType>().name;
  }


  template<TgTypes ParamType>
  constexpr void setParam(ParamType& param){
    if(checkContent<ParamType>("setParam")){
      parameters.template at<ParamType>().set(param.get());
    }
  }

  template <TgTypes... Params>
  constexpr void setParams(Params&... params) {
    (setParam(params), ...);
  }

protected:
  template<typename T>
  bool checkContent(std::string method_name = ""){
    std::string error_msg = "invalid argument";
    if(!method_name.empty()){
      method_name = " " + method_name;
    }

    if(!parameters.template contains<T>()){
      throw std::invalid_argument(error_msg + method_name);
      return false;
    }
    return  true;
  }

  ParametersContainer<AvailableTypes...> parameters;
};

} //core
} //telegram
