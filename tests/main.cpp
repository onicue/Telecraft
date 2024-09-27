#include "telecraft/api/params.hpp"
#include <telecraft/api/methods.hpp>
#include <telecraft/core/RequestManager.hpp>

#include <iostream>

#define TOKEN "token"
#define CHATID 131

using namespace telegram;

int main(){

  core::RequestComponentManager director;
  methods::SendMessage* sendMessage = new methods::SendMessage;
  try{
    director.fields.token = TOKEN;
    director.fields.connection = "close";

    sendMessage->set<param::text>("fafafa");
    sendMessage->set<param::chat_id>(CHATID);

    director.newMethod(sendMessage);
    director.buildAll();

    std::string message = director.generateHTTP();
    std::cout << message << std::endl;

    std::string body = sendMessage->getBody();
    sendMessage->set<param::text>("kfkfkfkfk");
    sendMessage->set<param::chat_id>(313);
    sendMessage->deserialize(body);
    sendMessage->build();
    std::string body_2 =  sendMessage->getBody();
    if(body ==body_2){
      std::cout << "PASSED" << std::endl;
    } else {
      std::cout << "Faild" << std::endl;
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  delete sendMessage;
  return 0;
}
