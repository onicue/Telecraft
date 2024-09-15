#pragma once
#include <glaze/json/json_t.hpp>
#include <type_traits>
#include <glaze/glaze.hpp>
#include "../core/ParamsManagement.hpp"
#include <iostream>

namespace telegram {
namespace core {
  template<TgTypes... AvailableTypes>
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

  template<typename T>
  typename TypeMapper<T>::type convert(T value) {
    using type = typename TypeMapper<T>::type;
    if constexpr ( std::is_same_v<T,type>) {
      return value;
    } else {
      return static_cast<type>(value);
    }
  }

  template<TgTypes... T>
  std::string serialize(core::ParamManager<T...>& mngr){
    auto& container = mngr.parameters;
    object_t obj;
    std::string buffer;

    container.forEach([&obj, &container](const auto& param){
      if constexpr (!container.isMethod(param)){
        obj[param.getName()] = param.get();
      } else {
        //TODO
        std::cerr << "Error: Method parameters are not yet supported. This feature will be added soon." << std::endl;
      }
    });

    auto err = glz::write<glz::opts{}>(obj, buffer);
    if (err) {
      std::cerr << "JSON write error: " << err.custom_error_message << std::endl;
    }
    return buffer;
  }

  template<TgTypes... T>
  bool deserialize(core::ParamManager<T...>& mngr, const std::string& json){
    auto& container = mngr.parameters;
    object_t obj;

    auto err = glz::read_json(obj, json);
    if (err) {
      std::cerr << "JSON parsing error: " << err.custom_error_message << std::endl;
      return false;
    }

    for (auto it = obj.begin(); it != obj.end(); ++it) {
      const auto& key = it->first;
      const auto& value = it->second;
      try {
        auto& param = container.at(key);
        if constexpr (!container.isMethod(param)) {
          using ParamType = typename TypeMapper<typename std::decay_t<decltype(param)>::ValueType>::type;
          param.set(value.get<ParamType>());
        } else {
          //TODO
          std::cerr << "Error: Method parameters are not yet supported. This feature will be added soon." << std::endl;
        }
      } catch (const std::out_of_range& e) {
        std::cerr << "Key '" << key << "' not found in container. Value: " << value.get<std::string>() << std::endl;
      } catch (const std::exception& e) {
        std::cerr << "Error setting value for key '" << key << "': " << e.what() << std::endl;
      }
    }

    return true;
  }
} //json
} //telegram
