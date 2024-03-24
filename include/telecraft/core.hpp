#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "types_core.hpp"
#include "types.hpp"
#include <iostream>

namespace telegram{
namespace core{
class Message{
  public:
    std::string body; //HTTP query body
};

class QueryBuilder{
  public:
    virtual ~QueryBuilder(){};
    virtual void build() = 0;
    /* virtual void serializeMessageToJson(const string& message) = 0; */
    virtual std::string getHttpBody() = 0;
};

template<telegram::types::TgTypes... Args>
class BuilderManager{
  public:
    template<telegram::types::TgTypes ParamType, typename ValueType = ParamType::ValueType>
    constexpr ValueType get() {
      if (parameters.template contains<ParamType>()) {
        return parameters.template at<ParamType>().get();
      } else {
        throw std::invalid_argument("Invalid argument in method get");
      }
    }

    template<telegram::types::TgTypes ParamType, typename ValueType = ParamType::ValueType>
    constexpr void set(const ValueType& value) {
      if (parameters.template contains<ParamType>()) {
        parameters.template at<ParamType>().set(value);
      } else {
        throw std::invalid_argument("Invalid argument in method get");
      }
    }

    template<types::core::TgTypeEntity ParamType>
    constexpr std::string getParameterName() {
      return parameters.template at<ParamType>().name;
    }

  protected:
    types::core::ParametersContainer<Args...> parameters;
};

class SendMessage : QueryBuilder, public BuilderManager<types::text, types::chat_id>{
  public:
    virtual void build() override {
        std::cout << "Building message...\n";
    }

    virtual std::string getHttpBody () override {
      return "gaagga";
    }
};

class TelegramAPI{
  public:
    TelegramAPI(std::string bot_token) : BotToken(bot_token) {}
  private:
    //header parameters   connection/size/
    std::string BotToken;
    QueryBuilder* builder;
};

}//core
} //telegram
