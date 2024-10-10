#pragma once
#include "core/ClientInterface.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/verify_mode.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <memory>
#include <mutex>

namespace telecraft {
  using boost::asio::ip::tcp;
  using namespace std::placeholders;
  namespace ssl = boost::asio::ssl;

  class BoostClient {
  public:
    BoostClient(const std::string& bot_token, boost::asio::io_context& io_context, boost::asio::ssl::context& context_)
      : socket_(io_context, context_) {
      connect();
    }

    BoostClient(const std::string& bot_token, boost::asio::io_context& io_context)
      : BoostClient(bot_token, io_context, *new boost::asio::ssl::context(boost::asio::ssl::context::tlsv12_client)) {}

    void write(const std::string& message) {
      /*std::lock_guard<std::mutex> lock(mutex_);*/
      queue.push(message);
    }

  private:
    void connect() {
      try {
        tcp::resolver resolver(socket_.get_executor());
        auto endpoints = resolver.resolve(TG_HOST, TO_STRING(TG_PORT));

        boost::asio::async_connect(socket_.lowest_layer(), endpoints,
          [this](const boost::system::error_code& error, const tcp::endpoint& /*endpoint*/) {
            if (!error) {
              socket_.async_handshake(boost::asio::ssl::stream_base::client,
                [this](const boost::system::error_code& error) {
                  if (error) {
                    std::cout << "Handshake failed: " << error.message() << "\n";
                    return;
                  }
                  do_write();
                });
            } else {
              std::cout << "Connect failed: " << error.message() << "\n";
            }
          });

      } catch (const boost::system::system_error& e) {
        std::cerr << "Error during connection: " << e.what() << "\n";
      }
    }

    void do_write() {
      /*std::lock_guard<std::mutex> lock(mutex_);*/

      if (queue.empty()){
        return;
      }

      std::string message = queue.front();
      queue.pop();

      boost::asio::async_write(socket_, boost::asio::buffer(message),
        [this](const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
          if (!error) {
            handle_write_request(error);
          } else {
            std::cout << "Write failed: " << error.message() << "\n";
          }
        });
    }

    void handle_write_request(const boost::system::error_code& err) {
      if (!err) {
        auto response = std::make_shared<boost::asio::streambuf>(); // Create a shared buffer
        boost::asio::async_read_until(socket_, *response, "\r\n",
          std::bind(&BoostClient::handle_read_status_line, this, response, std::placeholders::_1));
      } else {
        std::cout << "Error: " << err.message() << "\n";
      }
    }

    void handle_read_status_line(std::shared_ptr<boost::asio::streambuf> response, const boost::system::error_code& err) {
      if (!err) {
        std::istream response_stream(response.get());
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);

        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
          std::cout << "Invalid response\n";
          return;
        }
        if (status_code != 200) {
          std::cout << "Response returned with status code " << status_code << "\n";
          return;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(socket_, *response, "\r\n\r\n",
          std::bind(&BoostClient::handle_read_headers, this, response, std::placeholders::_1));

      } else {
        std::cout << "Error: " << err.message() << "\n";
      }
    }

    void handle_read_headers(std::shared_ptr<boost::asio::streambuf> response, const boost::system::error_code& error) {
      if (!error) {
        std::istream response_stream(response.get());
        std::string header;
        while (std::getline(response_stream, header) && header != "\r") {
          std::cout << header << "\n";
        }
        std::cout << "\n";

        // Write whatever content we already have to output.
        if (response->size() > 0) {
          std::cout << response.get();
        }

        // Start reading remaining data until EOF.
        boost::asio::async_read(socket_, *response,
          boost::asio::transfer_at_least(1),
          std::bind(&BoostClient::handle_read_content, this, response, std::placeholders::_1));
      } else {
        std::cout << "Error: " << error.message() << "\n";
      }
    }

    void handle_read_content(std::shared_ptr<boost::asio::streambuf> response, const boost::system::error_code& error) {
      if (!error) {
        std::cout.write(boost::asio::buffer_cast<const char*>(response->data()), response->size());
        std::cout << std::flush;

        response->consume(response->size());

        boost::asio::async_read(socket_, *response,
          boost::asio::transfer_at_least(1),
          std::bind(&BoostClient::handle_read_content, this, response, std::placeholders::_1));
      } else if (error == ssl::error::stream_truncated ||
                 error == boost::asio::error::eof) {
        do_write();
      } else if (error != boost::asio::error::eof) {
        std::cout << "Error: " << error.message() << "\n";
      }
    }

    boost::asio::ssl::stream<tcp::socket> socket_;
    std::queue<std::string> queue;
    std::mutex mutex_;
    bool writing_ = false;
  };
}
