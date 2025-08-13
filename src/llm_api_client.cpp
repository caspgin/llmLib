#include "llm_api_client.h"
#include "nlohmann/json_fwd.hpp"
#include <cstdlib>
#include <stdexcept>
#include <string>
#define CPPHTTBLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "nlohmann/json.hpp"

const std::string ENV_VAR_NAME = "GEMINI_API_KEY";

std::string GetApiKey() {
  const char *enviKey = std::getenv(ENV_VAR_NAME.c_str());

  if (enviKey == nullptr || std::string(enviKey).length() == 0) {
    throw std::runtime_error("Did not find environment key. Please a \"" +
                             ENV_VAR_NAME +
                             "\" environment vairable for your AI key.\n");
  }

  return std::string(enviKey);
}

std::string CallGemini(const std::string &prompt) {
  const std::string APIKEY = GetApiKey();
  const std::string LLM_DOMAIN = "generativelanguage.googleapis.com";
  const std::string LLM_PATH =
      "/v1beta/models/gemini-2.0-flash:generateContent";

  httplib::SSLClient cli(LLM_DOMAIN);

  httplib::Headers headers = {{"Content-Type", "application/json"},
                              {"X-goog-api-key", APIKEY}};

  nlohmann::json jsonBody;
  jsonBody["contents"] = {{{"parts", {{"text", prompt}}}}};
  std::string responseText = "";
  if (auto res = cli.Post(LLM_PATH.c_str(), headers, jsonBody.dump(),
                          "application/json")) {
    if (res->status == 200) {
      try {
        nlohmann::json response = nlohmann::json::parse(res->body);
        return response["candidates"][0]["content"]["parts"][0]["text"];
      } catch (const nlohmann::json::parse_error &e) {
        std::string error = "Parsing Error: " + std::string(e.what());
        throw CustomError(error, "Parsing Error");
      }
    } else {
      throw CustomError(std::to_string(res->status), "ResponseError");
    }
  } else {
    auto err = res.error();
    std::string except = "Connection Error: " + httplib::to_string(err);
    throw CustomError(except, "Connection Error");
  }

  return responseText;
}
