#pragma once

constexpr const char* TG_HOST = "api.telegram.org";
constexpr int TG_PORT = 433;

namespace telegram {
namespace core {
  class BaseMethod;

  class IClient {
    public:
      virtual void close() = 0;
      virtual ~IClient(){}

      virtual void wite(BaseMethod* method);
    private:

  };
}
}
