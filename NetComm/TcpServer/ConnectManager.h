#pragma once


#include <evpp/sockets.h>
#include <evpp/tcp_server.h>
#include <evpp/tcp_conn.h>

#include <map>
#include <mutex>
#include <thread>

typedef std::function<void(int)> MyConnectionCallback;

template<class T>
class ConnectManager {
public:
	ConnectManager(int threadNum);
	~ConnectManager();

public:
	bool start(std::string addr);
	bool stop();

	void sendToAllClients(char* data, size_t len);
	void sendToClientBySocketType(int socketType, char* data, size_t len);
	void sendToClientByFd(evpp_socket_t fd, char* data, size_t len);

	void setNewConnectionCallback(MyConnectionCallback cb) {
		m_newConCb = cb;
	}

	void setConnectionCloseCallback(MyConnectionCallback cb) {
		m_conCloseCb = cb;
	}

private:
	void startInternal();
	void onReceiveMsg(const evpp::TCPConnPtr& conn, evpp::Buffer* msg);
	void onNewConnection(const evpp::TCPConnPtr& conn);
	void onConnectionClose(const evpp::TCPConnPtr& conn);
	void addClient(evpp::TCPConnPtr conn);
	void removeClient(evpp_socket_t fd);
	std::shared_ptr<T> findClientByFd(evpp_socket_t fd);

private:
	typedef std::map<int/*the id of the connection*/, std::shared_ptr<T>> ConnectionMap;
	ConnectionMap m_connections;
	std::mutex m_lock;

	int m_threadNum;
	std::atomic<bool> m_started;
	std::shared_ptr<evpp::TCPServer> m_tcpServer;
	evpp::EventLoop m_loop;
	std::shared_ptr<std::thread> m_thread;

	MyConnectionCallback m_newConCb;
	MyConnectionCallback m_conCloseCb;
};

template <class T>
ConnectManager<T>::ConnectManager(int threadNum) {
	m_threadNum = threadNum;
	m_started.store(false);
}

template <class T>
ConnectManager<T>::~ConnectManager() {
	if (m_started.load()) {
		stop();
		m_started.store(false);
	}
}

template <class T>
bool ConnectManager<T>::start(std::string addr) {
	if (m_started.load()) {
		//LOG_INFO << "tcp server already started.";
		return true;
	}
	m_tcpServer = std::make_shared<evpp::TCPServer>(&m_loop, addr, "TCPServer", m_threadNum);
	ConnectManager<T>* manager = this;
	m_thread = std::make_shared<std::thread>([manager](void* ptr) {
		manager->startInternal();
	}, this);
	m_started.store(true);
	return true;
}

template <class T>
void ConnectManager<T>::startInternal() {
	ConnectManager<T>* manager = this;
	m_tcpServer->SetMessageCallback([manager](const evpp::TCPConnPtr& conn,
		evpp::Buffer* msg) {
		manager->onReceiveMsg(conn, msg);
	});
	m_tcpServer->SetConnectionCallback([manager](const evpp::TCPConnPtr& conn) {
		if (conn->IsConnected()) {
			manager->onNewConnection(conn);
		}
		else {
			manager->onConnectionClose(conn);
		}
	});
	m_tcpServer->Init();
	m_tcpServer->Start();
	m_loop.Run();
}

template <class T>
void ConnectManager<T>::onReceiveMsg(const evpp::TCPConnPtr& conn, evpp::Buffer* msg) {
	int fd = conn->fd();
	std::shared_ptr<T> wrapConn = findClientByFd(fd);
	if (wrapConn != nullptr) {
		wrapConn->onMsg(msg);
	}
	else {
		//LOG_WARN << "receive message from unkown client : " << fd;
	}
}

template <class T>
void ConnectManager<T>::onNewConnection(const evpp::TCPConnPtr& conn) {
	if (m_newConCb) {
		m_newConCb(conn->fd());
	}
	addClient(conn);
}

template <class T>
void ConnectManager<T>::onConnectionClose(const evpp::TCPConnPtr& conn) {
	if (m_conCloseCb) {
		m_conCloseCb(conn->fd());
	}
	removeClient(conn->fd());
}

template <class T>
bool ConnectManager<T>::stop() {
	if (!m_started.load()) {
		//LOG_INFO << "tcp server is not started.";
		return true;
	}
	m_loop.Stop();
	m_thread->join();
	m_started.store(false);
	return true;
}

template <class T>
void ConnectManager<T>::addClient(evpp::TCPConnPtr conn) {
	std::shared_ptr<T> tcpConn = std::make_shared<T>(conn);
	std::lock_guard<std::mutex> guard(m_lock);
	m_connections.insert(std::pair<int, std::shared_ptr<T>>(conn->fd(), tcpConn));
}

template <class T>
void ConnectManager<T>::removeClient(evpp_socket_t fd) {
	std::lock_guard<std::mutex> guard(m_lock);
	m_connections.erase(fd);
}

template <class T>
void ConnectManager<T>::sendToAllClients(char* data, size_t len) {
	std::lock_guard<std::mutex> guard(m_lock);
	for (auto& c : m_connections) {
		if (c.second->getTcpConn()->IsConnected()) {
			c.second->getTcpConn()->Send(data, len);
		}
	}
}

template <class T>
void ConnectManager<T>::sendToClientBySocketType(int socketType, char* data, size_t len) {
	std::lock_guard<std::mutex> guard(m_lock);
	for (auto& c : m_connections) {
		if (c.second->getTcpConn()->IsConnected() && c.second->getSocketType() == socketType) {
			c.second->getTcpConn()->Send(data, len);
		}
	}
}

template <class T>
void ConnectManager<T>::sendToClientByFd(evpp_socket_t fd, char* data, size_t len) {
	std::lock_guard<std::mutex> guard(m_lock);
	typename ConnectionMap::iterator iter = m_connections.find(fd);
	if (iter != m_connections.end())
	{
		iter->second->getTcpConn()->Send(data, len);
	}
}

template <class T>
std::shared_ptr<T> ConnectManager<T>::findClientByFd(evpp_socket_t fd) {
	std::lock_guard<std::mutex> guard(m_lock);
	typename ConnectionMap::iterator iter = m_connections.find(fd);
	if (iter != m_connections.end())
	{
		return iter->second;
	}
	else {
		return nullptr;
	}
}
