#ifndef LLM_API_CLIENT
#define LLM_API_CLIENT

#include "error/LLMError.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

struct LlmResponse {
	enum class Status { RUNNING, FINISHED, FALIURE, WAITING };
	Status		status;
	std::string text;
	LLMError	error;
};

class LlmClient {

  public:
	LlmClient();
	~LlmClient();
	bool SendPrompt(const std::string& prompt);

	LlmResponse GetNextResponse();

  private:
	void		WorkerLoop();
	std::string GetApiKey();
	std::string CallGemini(const std::string&);
	void		CallGeminiStream(const std::string&						 prompt,
								 std::function<void(const std::string&)> onChunk,
								 std::function<void()>					 onComplete);

  private:
	std::thread				m_workerThread;
	std::mutex				m_mutex;
	std::condition_variable m_cv;
	std::atomic<bool>		m_stop;
	std::string				m_currentPrompt;
	std::queue<LlmResponse> m_responseQueue;
};

#endif // !LLM_API_CLIENT
