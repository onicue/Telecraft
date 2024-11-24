#pragma once
#include <string>
#include <vector>

#include "ClientInterface.hpp"

namespace telegram{
namespace core {
namespace http {

const std::string TC_ContentType[3] {"application/x-www-form-urlencoded",
                                     "application/json",
                                     "multipart/form-data"};

const std::string TC_Method[2] {"POST", "GET"};

enum class ContentType {
  UrlEncoded,
  Json,
  FormData
};

enum class Method {
  POST,
  GET
};

class HeaderManager {
public:
  HeaderManager(const std::string& token) : token(token) {
    addHost();
  }

  void addFieldToHeader(std::string field) {
    fields.push_back(field);
  }

  const std::vector<std::string>& getHeader(){
    return fields;
  }

  std::string getToken(){
    return token;
  }

  void setToken(const std::string& token){
    this->token = token;
  }

  void setHeader(const std::vector<std::string>& newHeader){
    fields = newHeader;
  }

  void clear() {
    fields.clear();
    addHost();
  }

  bool checkToken(){
    if (token.empty()) {
      return false;
    }
    return true;
  }
protected:
  void addHost(){
    std::string host = TG_HOST;
    addFieldToHeader("Host: " + host);
  }

  std::vector<std::string> fields;
  std::string token;
};

} //http
} //core
} //telegram
