#include "stdafx.h"
#include "EvppHttpServer.h"

#include <mutex>

CEvppHttpServer* CEvppHttpServer::getInstance() {
	static CEvppHttpServer* gHttpServer = NULL;
	static std::mutex lock;
	lock.lock();
	if (gHttpServer == NULL) {
		gHttpServer = new CEvppHttpServer();
	}
	lock.unlock();
	return gHttpServer;
}

bool CEvppHttpServer::start(uint32_t threadNum, std::vector<int> ports) {
	if (m_started.load()) {
		// http server started already
		return true;
	}
	m_httpServer = std::make_shared<evpp::http::Server>(threadNum);
	m_httpServer->Init(ports);
	m_httpServer->Start();
	m_started.store(true);
	return true;
}

bool CEvppHttpServer::stop() {
	if (!m_started.load()) {
		// http server is stopped already
		return true;
	}
	m_httpServer->Stop();
	m_started.store(false);
	return true;
}

void CEvppHttpServer::registerHandler(std::string path, evpp::http::HTTPRequestCallback callback) {
	m_httpServer->RegisterHandler(path, callback);
}

