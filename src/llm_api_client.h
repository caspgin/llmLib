#ifndef LLM_API_CLIENT
#define LLM_API_CLIENT

#include <functional>
#include <string>
std::string GetApiKey();
std::string CallGemini(const std::string&);
void		CallGeminiStream(const std::string&						 prompt,
							 std::function<void(const std::string&)> onChunk,
							 std::function<void()>					 onComplete);
#endif // !LLM_API_CLIENT
