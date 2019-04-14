#pragma once

#include <evpp/http/http_server.h>
#include <string>
#include <vector>

class CEvppHttpServer {
public:
	static CEvppHttpServer* getInstance();

private:
	CEvppHttpServer() {
		m_started.store(false);
	}

public:
	bool start(uint32_t threadNum, std::vector<int> ports);
	bool stop();
	void registerHandler(std::string path, evpp::http::HTTPRequestCallback callback);

private:
	uint32_t m_threadNum;
	std::atomic<bool> m_started;
	std::shared_ptr<evpp::http::Server> m_httpServer;
};

