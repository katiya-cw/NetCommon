#pragram once

#include "TcpClient.h"

class CTcpClientInstance {
	public:
		static CTcpClientInstance* getInstance() {
   	  static CTcpClientInstance* instance = NULL;
  	  static std::mutex lock;
  	  lock.lock();
  	  if (instance == NULL) {
  	    instance = new CTcpClientInstance();
  	  }
  	  lock.unlock();
  	  return instance;
		}

  private:
		bool start(std::string raddr, std::string name) {
			m_tcpClient.reset(new CTcpClient(raddr, name));
			return m_tcpClient->start();
		}

		bool stop() {
			return m_tcpCient->stop();
		}

		void setMessageCallback(MessageCallback messageCallback) {
			m_tcpClient->setMessageCallback(messageCallback);
		}

  private:
		std::shared_ptr<CTcpClient> m_tcpClient;
};

