#include <telecraft/api/methods.hpp>
#include <telecraft/RequestManager.hpp>

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

  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  delete sendMessage;
  return 0;
}
