#pragma once

#include "EvppHttpClient.h"

class CEvppHttpClientManager {
public:
	static CEvppHttpClientManager* getInstance() {
 	  static CEvppHttpClientManager* gHttpClient = NULL;
	  static std::mutex lock;
	  lock.lock();
	  if (gHttpClient == NULL) {
	    gHttpClient = new CEvppHttpClientManager();
	  }
	  lock.unlock();
	  return gHttpClient;
	}

public:
	std::string request(std::string url) {
		return m_httpClient.request(url);
	}

private:
	CEvppHttpClient m_httpClient;
};

