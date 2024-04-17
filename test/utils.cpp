#include "utils.h"
#include <iostream>

bool checkValidityRequest(std::string request){
  size_t end_first_line = request.find("\r\n");
  std::string first_line = request.substr(0, end_first_line);
  if(first_line.find("200") != std::string::npos){
    return true;
  }

  size_t error_code_pos = first_line.find(" ") + 1;
  std::string httpErrorCode = first_line.substr(error_code_pos, 3);
  handleError("Request is not valid. Error code: " + httpErrorCode);
  return false;
}

template<typename T, typename... Args>
void assertEquals(T first, Args... args) {
  bool areAllEqual = ((first == args) && ...);
  if(!areAllEqual){
    handleError("Not all arguments are equal");
  }
}

void handleError(const std::string& errorMessage){
  std::cerr << "Error: " << errorMessage << std::endl;
}
