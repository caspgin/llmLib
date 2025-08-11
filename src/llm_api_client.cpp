#include "llm_api_client.h"

#include <cstdlib>
#include <stdexcept>
#include <string>
#define CPPHTTBLIB_OPENSSL_SUPPORT
#include "httplib.h"
// std::string CreatePromptJson(std::string &prompt) { std::string outJson;
//
//

const std::string ENV_VAR_NAME = "GEMINI_API_KEY";

std::string GetApiKey() {
    const char* enviKey = std::getenv(ENV_VAR_NAME.c_str());

    if (enviKey == nullptr || std::string(enviKey).length() == 0) {
        throw std::runtime_error("Did not find environment key. Please a \"" +
                                 ENV_VAR_NAME +
                                 "\" environment vairable for your AI key.\n");
    }

    return std::string(enviKey);
}

void CallGemini() {
    const std::string apiKey = GetApiKey();

    httplib::SSLClient cli("generativelanguage.googleapis.com");

    // Step 3: Set up the headers, including Content-Type and the API key.
    httplib::Headers headers = {{"Content-Type", "application/json"},
                                {"X-goog-api-key", apiKey}};

    // Step 4: Define the JSON payload for the POST request.
    // This is the same data as in your curl command.
    std::string jsonBody = R"({
            "contents":[
                {"parts" : [{"text": "Explain How Ai works in one sentence"}]}
            ]
        })";

    // Step 5: Make a POST request with the specified path, headers, and body.
    // The path is everything after the host name in the URL.
    std::string path = "/v1beta/models/gemini-2.0-flash:generateContent";
    if (auto res =
            cli.Post(path.c_str(), headers, jsonBody, "application/json")) {
        if (res->status == 200) {
            std::cout << "Status: " << res->status << std::endl;
            std::cout << "Body: " << res->body << std::endl;
        } else {
            std::cerr << "HTTP Error: " << res->status << std::endl;
            std::cerr << "Response Body: " << res->body << std::endl;
        }
    } else {
        auto err = res.error();
        std::cerr << "Connection Error: " << httplib::to_string(err)
                  << std::endl;
    }
}
