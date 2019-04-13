#include "TcpClient.h"

CTcpClient::~CTcpClient() {
	if (IsRunning()) {
		stop();
	}
}

bool CTcpClient::start() {
	m_client->SetConnectionCallback([this](const evpp::TCPConnPtr& conn) {
			if (!conn->IsConnected()) {
			  m_client->Connect();
			}
	});
	m_client->SetMessageCallback([this](const evpp::TCPConnPtr& conn, evpp::Buffer* buffer) {
			if (buffer == NULL || buffer->data() == NULL || buffer->length() <= 0) {
			  return;
			}
			m_messageCallback(conn->fd(), buffer->data(), buffer->length());
			buffer->Skip(buffer->length());
	});
	m_client->Connect();
	bool started = m_loopThread.Start();
	if (started) {
		status_.store(evpp::ServerStatus::kRunning);
	} else {
		status_.store(evpp::ServerStatus::kStopped);
	}
	return started;
}

bool CTcpClient::stop() {
	if (!IsRunning()) {
		return true;
	}
	m_client->Disconnect();
	m_loopThread.Stop();
	m_loopThread.Join();
	status_.store(evpp::ServerStatus::kStopped);
	return true;
}

