#include <iostream>
#include <ostream>
#include <string>

#include "error/LLMError.h"
#include "llm_api_client.h"

int main() {
	std::string fullData;
	LlmClient	client;
	std::cout << "Test Start" << std::endl;
	bool prompted = client.SendPrompt(
		"Get me a list of most common tools in kitchen in both "
		"english and french. no more than 5 items.");
	if (!prompted) {
		std::cout << "Prompt did not work" << std::endl;
		return -1;
	}
	bool loop = true;
	while (loop) {
		LlmResponse res = client.GetNextResponse();
		switch (res.status) {
		case LlmResponse::Status::RUNNING:
			fullData += res.text;
			break;
		case LlmResponse::Status::FALIURE:
			std::cout << res.error.getErrorType() << ":" << res.error.what()
					  << std::endl;
			loop = false;
			break;
		case LlmResponse::Status::WAITING:
			std::cout << "Waiting for Data." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			break;
		case LlmResponse::Status::FINISHED:
			std::cout << fullData;
			std::cout << "\nResponse Completed" << std::endl;
			loop = false;
			break;
		}
	}

	std::cout << "\nTest Complete" << std::endl;
	return 0;
}
