#pragma once

#include "HttpClient.h"

class CHttpClientManager {
public:
	static CHttpClientManager* getInstance() {
 	  static CHttpClientManager* gHttpClient = NULL;
	  static std::mutex lock;
	  lock.lock();
	  if (gHttpClient == NULL) {
	    gHttpClient = new CHttpClientManager();
	  }
	  lock.unlock();
	  return gHttpClient;
	}

public:
	std::string request(std::string url) {
		return m_httpClient.request(url);
	}

private:
	CHttpClient m_httpClient;
};

