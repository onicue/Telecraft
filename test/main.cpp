#include <telecraft/api/methods/sendMessage.hpp>
#include <telecraft/RequestManager.hpp>

/* #include <typeinfo> */
#include <iostream>

using namespace telegram;

int main(){
  core::RequestComponentManager director;
  methods::SendMessage* sendMessage = new methods::SendMessage;
  try{
    director.fields.token = "fafafaafafafafa";
    sendMessage->set<param::text>("afafafaafa");
    sendMessage->set<param::chat_id>(3131);
    director.newMethod(sendMessage);
    director.buildAll();
    std::cout << director.generateHTTP() << std::endl;
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  delete sendMessage;
  return 0;
}
