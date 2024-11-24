#pragma once
#include <string>
#include "HttpManagement.hpp"
#include "MethodBuilders.hpp"

namespace telegram {
namespace core {

class RequestGenerator{
public:
  static std::string generate(http::HeaderManager& header, BaseMethod* body){
    if(body == nullptr){
      throw std::invalid_argument("Body is null in RequestGenerator!");
    }

    try{
      generalCheck(header, body);
    } catch(const std::invalid_argument& e){
      std::cerr << e.what() << std::endl;
      throw std::runtime_error("Can't create request!");
      return "";
    }

    std::string request = buildRequestLine(header, body);
    request += buildHeaderFields(header);
    request += buildContentType(body);
    request += buildContentLength(body);
    request += buildBody(body);

    return request;
  };

private:
  static constexpr char fieldEnd[] = "\r\n";

  static inline void generalCheck(http::HeaderManager& header, BaseMethod* body) {
    if(header.getHeader().empty()){
      throw std::invalid_argument("ERROR: Header is empty in RequestGenerator!");
    }
  }

  static inline std::string buildRequestLine(http::HeaderManager& header, BaseMethod* body) {
    return http::TC_Method[static_cast<int>(body->getMethod())] + " " +
           "/bot" + header.getToken() + "/" + body->getName() +
           " HTTP/1.1" + fieldEnd;
  }

  static inline std::string buildHeaderFields(http::HeaderManager& header){
    std::string headerFields;
    for (auto field : header.getHeader()) {
      headerFields += field + fieldEnd;
    }

    return headerFields;
  }

  static inline std::string buildContentType(BaseMethod* body){
    return "Content-Type: " + http::TC_ContentType[static_cast<int>(body->getContentType())] + fieldEnd;
  }

  static inline std::string buildContentLength(BaseMethod* body){
    return "Content-Length: " + std::to_string(body->getBody().size()) + fieldEnd;
  }

  static inline std::string buildBody(BaseMethod* body){
    return fieldEnd + body->getBody();
  }
};

class RequestComponentManager{
public:
  RequestComponentManager(const std::string& token) : header(token){}

  http::HeaderManager header;

  void setMethod(BaseMethod* method){
    if(methodIsNull(method)){
      throw std::invalid_argument("Method is null in newMethod!");
    } else {
      this->method = method;
    }
  }

  void buildMethod(){
    if(methodIsNull(method)){
      throw std::invalid_argument("Building was stopped: Method is null in buildMethod!");
      return;
    }
    method->build();
  }

  std::string generateHTTP(){
    return RequestGenerator::generate(header, method);
  }

protected:
  bool methodIsNull(BaseMethod* method) { return method == nullptr; }
  BaseMethod* method;
};

} //core
} //telegram
