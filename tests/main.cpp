/*#include <telecraft/api/methods/sendMessage.hpp>*/
/*#include <telecraft/RequestManager.hpp>*/
/*#include "net/posix_ssl.h"*/
/*#include "utils.h"*/
/**/
/* #include <typeinfo> */
/*#include <iostream>*/
/**/
/*#define TOKEN ""*/
/*#define CHATID 131*/
/**/
/*using namespace telegram;*/
/**/
/*int main(){*/
/*  ClientSSL client;*/
/**/
/*  core::RequestComponentManager director;*/
/*  methods::SendMessage* sendMessage = new methods::SendMessage;*/
/*  try{*/
/*    director.fields.token = TOKEN;*/
/*    director.fields.connection = "close";*/
/**/
/*    sendMessage->set<param::text>("fafafa");*/
/*    sendMessage->set<param::chat_id>(CHATID);*/
/**/
/*    director.newMethod(sendMessage);*/
/*    director.buildAll();*/
/**/
/*    std::string message = director.generateHTTP();*/
/*    std::cout << message << std::endl;*/
/*    client.send(message);*/
/**/
/*    auto recv_message = client.recv();*/
/*    if(checkValidityRequest(recv_message)){*/
/*      std::cout << recv_message << std::endl;*/
/*    }*/
/*  } catch (const std::exception& e) {*/
/*    std::cout << e.what() << std::endl;*/
/*  }*/
/**/
/*  delete sendMessage;*/
/*  return 0;*/
/*}*/



#define BOOST_TEST_MODULE ExampleTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ExampleSuite)

BOOST_AUTO_TEST_CASE( my_test )
{

    BOOST_CHECK(true);
    BOOST_CHECK(1 == 1);
    BOOST_CHECK(1 >= 0);
    BOOST_CHECK(0 != 0);
}

BOOST_AUTO_TEST_SUITE_END()
