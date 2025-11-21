#include <iostream>
#include <ostream>
#include <string>

#include "error/LLMError.h"
#include "llm_api_client.h"

int main() {
	std::string fullData;
	try {
		std::cout << "Test Start" << std::endl;
		//	CallGemini("Make a list of ingredients for a simple apple pie");
		CallGeminiStream(
			"Get me a list of most common tools in kitchen in both english and "
			"french. no more than 5 items.",
			[&](const std::string& chunk) { fullData += chunk; },
			[&]() { std::cout << fullData << std::endl; });

		std::cout << "Test Complete" << std::endl;
	} catch (const LLMError& e) {
		std::cout << e.getErrorType() << "\t" << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "Caught a standard exception: " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Caught an unknown, non-standard error." << std::endl;
		return 1;
	}
	return 0;
}
