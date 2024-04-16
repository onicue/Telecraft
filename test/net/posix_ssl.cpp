#include "posix_ssl.h"
#include <iostream>
#include <netdb.h>

int ClientSSL::check_errors(int err){
  if(err < 0) {
    int err = SSL_get_error(ssl, err);

    if (err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE) {
      return 0; // Waiting for read or write operation to complete
    } else if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL) {
      return -1; // Closed connection or other critical SSL error
    } else {
      return -2;
    }
  }

  return 1; // Successful write operation
}

void ClientSSL::send(std::string message) {
  int err = SSL_write(ssl, message.c_str(), message.length());
  if(check_errors(err) < 0) {
    throw std::runtime_error( "Error writing to SSL. " + get_log_ssl());
  }
}

std::string ClientSSL::recv(){
  const int initialBufferSize = 1024;
  std::vector<char> buffer(initialBufferSize);
  return receiveData(buffer,0);
}

std::string ClientSSL::receiveData(std::vector<char>& buffer, int totalBytesReceived) {
  const int bufferSize = buffer.size();
  int bytesReceived = SSL_read(ssl, buffer.data() + totalBytesReceived, bufferSize - totalBytesReceived);

  if (check_errors(bytesReceived) < 0) {
    return "";
  } else if (bytesReceived == 0) {
    return std::string(buffer.data(), totalBytesReceived);
  } else {
    totalBytesReceived += bytesReceived;
    if (totalBytesReceived >= bufferSize) {
      buffer.resize(buffer.size() + bufferSize);
    }
    return receiveData(buffer, totalBytesReceived);
  }
}

std::string ClientSSL::get_log_ssl() {
  std::string error_messages;
  unsigned long err;
  while ((err = ERR_get_error()) != 0) {
    char *str = ERR_error_string(err, 0);
    if (str)
      error_messages += str;
  }
  return error_messages;
}

ClientSSL::ClientSSL() : addr(nullptr){
  try{
    init_addrinfo();
    int s = create_socket();
    connect_socket(s);
    init_ssl();
    init_ssl_connection(s);
  } catch (std::exception e) {
    std::cerr << "Error when attempting create ClientSSL: " << e.what() << std::endl;
  }
}

void ClientSSL::init_addrinfo() {
  struct addrinfo hint;
  memset(&hint, 0, sizeof(struct addrinfo));


  hint.ai_flags = 0; // AI_NUMERICHOST;
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = IPPROTO_TCP;

  int ret = getaddrinfo(DNSAddr, PORT, &hint, &addr);
  if (ret == EAI_NONAME) {
    std::cerr << "EAI_NONAME\n";
    hint.ai_flags = 0;
    ret = getaddrinfo(DNSAddr, PORT, &hint, &addr);
  }
  if (ret != 0) {
    throw std::runtime_error("Error in getaddrinfo");
  }
}

int ClientSSL::create_socket() {
  int s = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
  if (s == -1) {
    throw std::runtime_error("Error create socket\n");
  }
  return s;
}

void ClientSSL::connect_socket(int s) {
  if (connect(s, addr->ai_addr, addr->ai_addrlen) != 0) {
    throw std::runtime_error("Error connected\n");
  }
}

void ClientSSL::init_ssl() {
  SSL_library_init();
  SSLeay_add_ssl_algorithms();
  SSL_load_error_strings();

  const SSL_METHOD *meth = TLS_client_method();
  SSL_CTX *ctx = SSL_CTX_new(meth);
  ssl = SSL_new(ctx);
  if (!ssl) {
    throw std::runtime_error("Error creating ssl. " + get_log_ssl());
  }
}

void ClientSSL::init_ssl_connection(int s) {
  sockfd = SSL_get_fd(ssl);
  SSL_set_fd(ssl, s);
  int err = SSL_connect(ssl);
  if (err <= 0) {
    throw std::runtime_error("Error creating SSL connection. " + get_log_ssl());
  }
}

ClientSSL::~ClientSSL(){
  close(sockfd);
}
