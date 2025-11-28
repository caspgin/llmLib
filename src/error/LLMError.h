#ifndef LLMERROR_H
#define LLMERROR_H

#include <exception>
#include <string>
enum class LLMErroCode {
	MISSING_API_KEY,
	HTTP_ERROR,
	CONNECTION_ERROR,
	PARSE_ERROR,
	TIMEOUT_ERROR,
	UNKNOW,
	INVALID,
};

class LLMError : public std::exception {
  private:
	std::string message;
	std::string type;
	LLMErroCode code;

  public:
	LLMError() : message(""), type(""), code(LLMErroCode::INVALID) {};
	LLMError(const std::string& msg, const std::string& type, LLMErroCode code)
		: message(msg), type(type), code(code) {}

	const char*		  what() const noexcept override { return message.c_str(); }
	const LLMErroCode getErrorCode() const { return code; }
	const char*		  getErrorType() const { return type.c_str(); }
};

#endif
