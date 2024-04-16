#pragma once
#include <string>

#ifndef PORT
#define PORT  "443"
#endif

#ifndef DNSAddr
#define DNSAddr "api.telegram.org"
#endif

class Client {
  virtual void send(std::string http_request) = 0;
  virtual std::string recv() = 0;
};
