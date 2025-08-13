#ifndef LLM_API_CLIENT
#define LLM_API_CLIENT

#include <exception>
#include <string>
class CustomError : public std::exception {

public:
  std::string errorType;
  CustomError(const std::string &message, const std::string &errorType)
      : std::exception(message.c_str()) {
    this->errorType = errorType;
  }
};
std::string GetApiKey();
std::string CallGemini(const std::string &);
#endif // !LLM_API_CLIENT
