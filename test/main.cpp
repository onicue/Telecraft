#include <telecraft/core.hpp>
#include <telecraft/types.hpp>
#include <typeinfo>
#include <iostream>

int main(){
  telegram::core::SendMessage a;
  a.set<telegram::types::chat_id>(1111);
  auto b = a.get<telegram::types::chat_id>();
  std::cout << b << " " << typeid(b).name() << a.getParameterName<telegram::types::text>()<<std::endl;
  return 0;
}
