#pragma once
#include <string>
#include "MethodBuilders.hpp"
#include "core/http_mgmt.hpp"

namespace telegram {
namespace core {

class RequestGenerator{
  public:
    static std::string generate(http::HeaderManager& header, MethodBuilder* body){
      if(body == nullptr){
        throw std::invalid_argument("Body is null in RequestGenerator!");
      }

      try{
        generalCheck(header, body);
      } catch(const std::invalid_argument& ex){
        std::cerr << ex.what() << std::endl;
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
    static constexpr char fieldEnd[] = "\n\r";

    static void generalCheck(http::HeaderManager& header, MethodBuilder* body) {
      if(header.getHeader().empty()){
        throw std::invalid_argument("ERROR: Header is empty in RequestGenerator!");
      }
    }

    static std::string buildRequestLine(http::HeaderManager& header, MethodBuilder* body) {
      return http::TC_Method[static_cast<int>(body->getMethod())] +
             header.getHeader()[0] + "/" + body->getName() +
             " HTTP 1.1" + fieldEnd;
    }

    static std::string buildHeaderFields(http::HeaderManager& header){
      std::string headerFields;
      for (int i = 1; i < header.getHeader().size(); i++) {
        headerFields += header.getHeader()[i] + fieldEnd;
      }

      return headerFields;
    }

    static std::string buildContentType(MethodBuilder* body){
      return "Content-Type: " + http::TC_ContentType[static_cast<int>(body->getContentType())] + fieldEnd;
    }

    static std::string buildContentLength(MethodBuilder* body){
      return "Content-Length: " + std::to_string(body->getBody().size()) + fieldEnd;
    }

    static std::string buildBody(MethodBuilder* body){
      return body->getBody();
    }
};

class RequestComponentManager{
  public:
    http::HeaderFields fields;
    http::HeaderManager header;

    void newMethod(MethodBuilder* method){
      if(methodIsNull()){
        throw std::invalid_argument("Method is null in newMethod!");
      } else {
        this->method = method;
      }
    }

    void buildHeader(){
      http::HeaderBuilder::build(header, fields);
    }

    void buildMethod(){
      if(methodIsNull()){
        throw std::invalid_argument("Building was stopped: Method is null in buildMethod!");
        return;
      }
      method->build();
    }

    void buildAll(){
      buildHeader();
      buildMethod();
    }

    std::string generateHTTP(){
      return RequestGenerator::generate(header, method);
    }

  protected:
    bool methodIsNull() { return method == nullptr; }
    MethodBuilder* method;
};

} //core
} //telegram
