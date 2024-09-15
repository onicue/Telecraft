#pragma once
#include <type_traits>
#include <string>

namespace telegram {
namespace core {
  class BaseMethod;
  class BaseParameter;
}
namespace method {
  template<typename T>
  concept Type = std::is_base_of_v<core::BaseMethod, T>;
}
namespace param {
  template<typename T>
  concept Type = std::is_base_of_v<core::BaseParameter, T>;
}

template<typename T>
concept TgType = (method::Type<T> || param::Type<T>) &&
    requires (T obj) {
      { obj.get() } -> std::convertible_to<typename T::ValueType>;
      { obj.set(std::declval<decltype(obj.get())>()) };
      { obj.getName() } -> std::same_as<std::string>;
    };

} //telegram
