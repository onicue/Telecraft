#pragma once
#include "../../core/MethodBuilders.hpp"
#include "../params.hpp"
#include <nlohmann/json.hpp>

namespace telegram {
namespace methods {
class SendMessage : public telegram::core::MethodInfo<"sendMessage">, public core::ParamManager<param::chat_id, param::text> {
public:
  SendMessage(){}

  template<TgTypes... Args>
  SendMessage(Args... args) {
    setParams(args...);
  }

  virtual void build() override {
    auto chat_id = parameters.at<param::chat_id>();
    body[chat_id.name] = chat_id.get();
    auto text = parameters.at<param::text>();
    body[text.name] = text.get();
  }

  virtual std::string getBody () override {
    return body.dump();
  }
private:
  nlohmann::json body;
};

} //telegram
} //methods
