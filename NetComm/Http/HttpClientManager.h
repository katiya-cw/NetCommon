#pragma once

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
};

