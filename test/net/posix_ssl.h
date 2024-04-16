#pragma once
#include "client.h"
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <openssl/ssl.h>
#include <openssl/err.h>

class ClientSSL : Client {
public:
  ClientSSL();
  ~ClientSSL();

  virtual void send(std::string message) override;
  virtual std::string recv() override;

private:
  int sockfd;
  struct addrinfo *addr;
  SSL *ssl;

  std::string get_log_ssl();
  int check_errors(int err);

  void init_addrinfo();
  int create_socket();
  void connect_socket(int s);
  void init_ssl();
  void init_ssl_connection(int s);

  std::string receiveData(std::vector<char>& buffer, int totalBytesReceived);
};
