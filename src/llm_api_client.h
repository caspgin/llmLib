#ifndef LLM_API_CLIENT
#define LLM_API_CLIENT

#include <string>
std::string GetApiKey();
std::string CallGemini(const std::string &);
#endif // !LLM_API_CLIENT
