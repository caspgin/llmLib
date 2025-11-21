#include "llm_api_client.h"
#include "error/LLMError.h"
#include "nlohmann/json_fwd.hpp"
#include "openssl/x509.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#define CPPHTTBLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "nlohmann/json.hpp"

const std::string ENV_VAR_NAME = "GEMINI_API_KEY";

std::string GetApiKey() {
	const char* enviKey = std::getenv(ENV_VAR_NAME.c_str());

	if (enviKey == nullptr || std::string(enviKey).length() == 0) {
		throw LLMError("Did not find environment key. Please add  \"" +
						   ENV_VAR_NAME +
						   "\" environment vairable for your AI key.\n",
					   "MissingApiKey", LLMErroCode::MISSING_API_KEY);
	}

	return std::string(enviKey);
}

std::string CallGemini(const std::string& prompt) {
	const std::string APIKEY	 = GetApiKey();
	const std::string LLM_DOMAIN = "generativelanguage.googleapis.com";
	const std::string LLM_PATH =
		"/v1beta/models/gemini-2.0-flash:generateContent";

	httplib::SSLClient cli(LLM_DOMAIN);

	httplib::Headers headers = {{"Content-Type", "application/json"},
								{"X-goog-api-key", APIKEY}};

	nlohmann::json jsonBody;
	jsonBody["contents"]	 = {{{"parts", {{"text", prompt}}}}};
	std::string responseText = "";
	if (auto res = cli.Post(LLM_PATH.c_str(), headers, jsonBody.dump(),
							"application/json")) {
		if (res->status == 200) {
			try {
				nlohmann::json response = nlohmann::json::parse(res->body);
				return response["candidates"][0]["content"]["parts"][0]["text"];
			} catch (const nlohmann::json::parse_error& e) {
				throw LLMError(std::string(e.what()), "Parsing Error",
							   LLMErroCode::PARSE_ERROR);
			}
		} else {
			throw LLMError(std::to_string(res->status), "Http Response Error",
						   LLMErroCode::HTTP_ERROR);
		}
	} else {
		const auto err = res.error();
		switch (err) {
		case httplib::Error::SSLConnection:
			throw LLMError(
				"SSL Connection failed " + std::to_string(res.ssl_error()),
				"SSL Connection Error", LLMErroCode::CONNECTION_ERROR);
		case httplib::Error::SSLLoadingCerts:
			throw LLMError("SSL cert loading failed, OPENSSL error " +
							   std::to_string(res.ssl_openssl_error()),
						   "OPENSSL Certification",
						   LLMErroCode::CONNECTION_ERROR);
		default:
			throw LLMError(httplib::to_string(err), "Unknown httplib Error",
						   LLMErroCode::UNKNOW);
		}
	}

	return responseText;
}

void CallGeminiStream(const std::string&					  prompt,
					  std::function<void(const std::string&)> onChunk,
					  std::function<void()>					  onComplete) {

	const std::string APIKEY	 = GetApiKey();
	const std::string LLM_DOMAIN = "generativelanguage.googleapis.com";
	const std::string LLM_PATH =
		"/v1beta/models/gemini-2.0-flash:streamGenerateContent";

	httplib::SSLClient cli(LLM_DOMAIN);

	httplib::Headers headers = {{"Content-Type", "application/json"},
								{"X-goog-api-key", APIKEY}};

	bool		   firstChunk = true;
	nlohmann::json jsonBody;
	jsonBody["contents"] = {{{"parts", {{"text", prompt}}}}};
	auto res			 = cli.Post(
		LLM_PATH, headers, jsonBody.dump(), "application/json",
		[&](const char* data, size_t data_length) -> bool {
			try {
				std::string rawChunk(data, data_length);
				if (firstChunk) {
					if (rawChunk.front() == '[') {
						rawChunk.erase(0, 1);
					}
					firstChunk = false;
				}
				if (!rawChunk.empty() && rawChunk.front() == ',') {
					rawChunk.erase(0, 1);
				}
				if (!rawChunk.empty() && rawChunk.back() == ']') {
					rawChunk.pop_back();
				}
				if (!rawChunk.empty() && rawChunk.back() == ',') {
					rawChunk.pop_back();
				}
				if(rawChunk.empty()){
					return true;
				}
				nlohmann::json response = nlohmann::json::parse(rawChunk);
				std::string	   resBody =
					response["candidates"][0]["content"]["parts"][0]["text"];
				onChunk(resBody);
				return true;
			} catch (const nlohmann::json::parse_error& e) {
				throw LLMError(std::string(e.what()), "Parsing Error",
										   LLMErroCode::PARSE_ERROR);
			}
			
		},
		nullptr);

	if (res && res->status == 200) {
		onComplete();
		return;
	} else {
		const auto err = res.error();
		switch (err) {
		case httplib::Error::SSLConnection:
			throw LLMError(
				"SSL Connection failed " + std::to_string(res.ssl_error()),
				"SSL Connection Error", LLMErroCode::CONNECTION_ERROR);
		case httplib::Error::SSLLoadingCerts:
			throw LLMError("SSL cert loading failed, OPENSSL error " +
							   std::to_string(res.ssl_openssl_error()),
						   "OPENSSL Certification",
						   LLMErroCode::CONNECTION_ERROR);
		default:
			throw LLMError(httplib::to_string(err), "Unknown httplib Error",
						   LLMErroCode::UNKNOW);
		}
	}
}
