#pragma once

#include <evpp/tcp_conn.h>
#define SERVICE_IMPLEMENT
class ConnectBase {
public:
	ConnectBase(evpp::TCPConnPtr tcpConn) : m_tcpConn(tcpConn) {
		m_fd = m_tcpConn->fd();
	}
	virtual ~ConnectBase() = default;

public:
	virtual void onMsg(evpp::Buffer* msg) = 0;
	void send(const void* data, size_t len) {
		m_tcpConn->Send(data, len);
	}

public:
	void setSocketType(int socketType) {
		m_socketType = socketType;
	}
	int getSocketType() {
		return m_socketType;
	}
	evpp::TCPConnPtr getTcpConn() {
		return m_tcpConn;
	}

protected:
	int m_fd;

private:
	evpp::TCPConnPtr m_tcpConn;
	int m_socketType;
};
