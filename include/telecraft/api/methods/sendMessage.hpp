#pragma once
#include "../../core/MethodBuilders.hpp"
#include "../../utils/json.hpp"
#include "../params.hpp"
#include <glaze/glaze.hpp>

namespace telegram {
namespace methods {

class SendMessage : public telegram::core::MethodInfo<"sendMessage">, public core::ParamManager<param::chat_id, param::text> {
public:
  SendMessage(){}

  template<TgType... Args>
  SendMessage(Args... args) {
    setParams(args...);
  }

  virtual void build() override {
    /*auto chat_id = parameters.at<param::chat_id>();*/
    /*body[chat_id.getName()] = chat_id.get();*/
    /*auto text = parameters.at<param::text>();*/
    /*body[text.getName()] = text.get();*/
    body = json::serialize(*this);
  }

  virtual std::string getBody () override {
    /*std::string buffer;*/
    /*auto err = glz::write< glz::opts{ .prettify = false }>(body, buffer);*/
    /*if (err) {*/
    /*  std::cout << err.custom_error_message << std::endl;*/
    /*}*/
    /*return buffer;*/
    return body;
  }
private:
  /*glz::json_t::object_t body;*/
  std::string body;
};

} //telegram
} //methods
