#pragram once

#include <evpp/tcp_client.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>
#include <evpp/event_loop_thread.h>

// fd, data, size
typedef std::function<void(int, char*, int)> MessageCallback

class CTcpClient : public ServerStatus {
	public:
		CTcpClient(std::string raddr, std::string name):m_raddr(raddr),m_name(name) {
			m_client.reset(new evpp::TCPClient(m_loopThread.loop(), m_raddr, m_name));
			status_.store(evpp::kInitialized);
		}
		~CTcpClient();

  public:
		bool start();
		bool stop();
		void setMessageCallback(MessageCallback messageCallback) {
			m_messageCallback = messageCallback;
		}

	private:
		std::string m_name;
		std::string m_raddr;
		evpp::EventLoopThread m_loopThread;
		std::shared_ptr<evpp::TCPClient> m_client;
		MessageCallback m_messageCallback;
};

