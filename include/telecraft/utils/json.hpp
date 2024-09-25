#pragma once
#include <glaze/json/json_t.hpp>
#include <stdexcept>
#include <type_traits>
#include <glaze/glaze.hpp>
#include "TgTypes.hpp"

namespace telegram {
namespace core {
  template<TgType... AvailableTypes>
  class ParamManager;
}

namespace json {

using object_t = glz::json_t::object_t;

template<typename T>
struct TypeMapper{
  using type = T;
};

template<>
struct TypeMapper<int>{
  using type = double;
};

template<>
struct TypeMapper<long int>{
  using type = double;
};

template<typename T>
typename TypeMapper<T>::type convert(T value) {
  using type = typename TypeMapper<T>::type;
  if constexpr ( std::is_same_v<T,type>) {
    return value;
  } else {
    return static_cast<type>(value);
  }
}

template<TgType... T>
std::string serialize(core::ParamManager<T...>& mngr){
  auto& container = mngr.parameters;
  object_t obj;
  std::string buffer;

  container.forEach([&obj, &container](const auto& param){
    if constexpr (!container.isMethod(param)){
      obj[param.getName()] = param.get();
    } else {
      //TODO
      throw std::runtime_error("Method parameters are not yet supported. This feature will be added soon.");
    }
  });

  auto err = glz::write<glz::opts{}>(obj, buffer);
  if (err) {
    throw std::runtime_error("JSON write error: " + std::string(err.custom_error_message));
  }
  return buffer;
}

template<TgType... T>
void deserialize(core::ParamManager<T...>& mngr, const std::string& json){
  auto& container = mngr.parameters;
  object_t obj;

  auto err = glz::read_json(obj, json);
  if (err) {
    throw std::runtime_error( "JSON parsing error: " + std::string(err.custom_error_message));
    return;
  }

  for (const auto& [key, value] : obj) {
    try {
      auto& param = container.at(key);
      if (!container.isMethod(param)) {
        param.deserialize(value);
      } else {
        throw std::runtime_error("Method parameters are not yet supported. This feature will be added soon.");
      }
    } catch (const std::out_of_range&) {
      throw std::runtime_error("Key '" + key + "' not found in container.");
    } catch (const std::exception& e) {
      throw std::runtime_error("Setting value for key '" + key + "': " + std::string(e.what()));
    }
  }
}

} //json
} //telegram
