#pragma once

#ifndef TG_HOST
#define TG_HOST "api.telegram.org"
#endif

#ifndef TG_PORT
#define TG_PORT 443
#endif

#ifndef TO_STRING_HELPER
#define TO_STRING_HELPER(x) #x
#endif

#ifndef TO_STRING
#define TO_STRING(x) TO_STRING_HELPER(x)
#endif

namespace telegram {
namespace core {
  class IClient {
    public:
      virtual void close() = 0;
  };
}
}
