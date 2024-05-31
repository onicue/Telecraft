#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

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

//Struct to hold common header fields for building HTTP Telegram requests
//Can include any fields instead of Content-Type, method (POST or GET) and Content-Lenght
struct HeaderFields{
  std::string connection;
  std::string token;
  const std::string host = "api.telegram.org";
};

class FieldValidator {
public:
  void validateToken(const std::string& token) const {
    if (token.empty()) {
      throw std::invalid_argument("Token is empty. Please add a token.");
    }
  }

  void validateConnection(const std::string& connection) const {
    if (!connection.empty() && connection != "close" && connection != "keep-alive") {
      throw std::invalid_argument("Incorrect connection field! Please change the connection.");
    }
  }
};

class HeaderManager {
public:
  void addFieldToHeader(std::string field) {
    header.push_back(field);
  }

  inline const std::vector<std::string>& getHeader(){
    return header;
  }

  inline void setHeader(const std::vector<std::string>& newHeader){
    header = newHeader;
  }

  void clear() {
    header.clear();
  }
protected:
   std::vector<std::string> header;
};

class HeaderBuilder{
public:
  static void build(HeaderManager& header, HeaderFields& fields){
    if (checkFields(fields)) {
      buildHeader(header, fields);
    } else {
      throw std::runtime_error("Header wasn't built");
    }
  }

private:
  static bool checkFields(const HeaderFields& fields){
    try{
      FieldValidator fieldValidator;
      fieldValidator.validateToken(fields.token);
      fieldValidator.validateConnection(fields.connection);
    } catch(const std::invalid_argument& e){
      std::cerr << "Error occurred while checking fields: " << e.what() << std::endl;
      return false;
    }
    return true;
  }

  static void buildHeader(HeaderManager& header, const HeaderFields& fields){
    header.clear();
    header.addFieldToHeader("/bot" + fields.token);
    header.addFieldToHeader("Host: " + fields.host);

    if(!fields.connection.empty()){
      header.addFieldToHeader("Connection: " + fields.connection);
    }
  }
};

} //http
} //core
} //telegram
