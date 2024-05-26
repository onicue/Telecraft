#include <string>

bool checkValidityRequest(std::string request);

template<typename T, typename... Args>
void assertEquals(T first, Args... args);

void handleError(const std::string& errorMessage);
