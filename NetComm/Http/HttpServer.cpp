#include "HttpServer.h"

#include <mutex>

CHttpServer* CHttpServer::getInstance() {
	static CHttpServer* gHttpServer = NULL;
	static std::mutex lock;
	lock.lock();
	if (gHttpServer == NULL) {
		gHttpServer = new CHttpServer();
	}
	lock.unlock();
	return gHttpServer;
}

bool CHttpServer::start(uint32_t threadNum, std::vector<int> ports) {
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

bool CHttpServer::stop() {
	if (!m_started.load()) {
		// http server is stopped already
		return true;
	}
	m_httpServer->Stop();
	m_started.store(false);
	return true;
}

void CHttpServer::registerHandler(std::string path, evpp::http::HTTPRequestCallback callback) {
	m_httpServer->RegisterHandler(path, callback);
}

