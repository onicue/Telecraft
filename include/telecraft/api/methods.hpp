#pragma once
#include "../core/MethodBuilders.hpp"
#include "params.hpp"
#include "telecraft/utils/TgTypes.hpp"
#include "telecraft/utils/json.hpp"
#include <glaze/glaze.hpp>

namespace telegram {
namespace methods {
const auto ContentType_d = core::http::ContentType::Json;
const auto Method_d = core::http::Method::POST;

template<StringLiteral name,
         core::http::ContentType content_type,
         core::http::Method method,
         TgType... params>
class MethodBuilder : public core::MethodInfo<name, content_type, method>, public core::ParamManager<params...>{
public:
  template<TgType... Args>
  MethodBuilder(Args... args) {
    setParams(args...);
  }
  MethodBuilder(){}

  MethodBuilder(const MethodBuilder<name, content_type, method, params...>& other) {
    this->body = other.getBody();
  }

  void build() override {
    switch(content_type) {
      case core::http::ContentType::Json:
        body = json::serialize(*this);
        break;
      default:
        std::cerr << "can't build body"<< std::endl;
        throw std::invalid_argument("In function \"build()\" cannot identify content type. ContentTypecode: " + std::to_string(static_cast<int>(content_type)));
    }
  }

  std::string getBody() override {
    return body;
  }
private:
  std::string body;
};

class SendMessage : public MethodBuilder<"SendMessage", ContentType_d, Method_d, param::chat_id, param::text> {};

} //telegram
} //methods