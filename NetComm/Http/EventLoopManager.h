#pragma once

#include <mutex>
#include <evpp/event_loop.h>

class CEventLoopManager {
public:
	static CEventLoopManager* getInstance() {
		static CEventLoopManager* instance = NULL;
		static std::mutex lock;
		lock.lock();
		if (instance == NULL) {
			instance = new CEventLoopManager();
		}
		lock.unlock();
		return instance;
	}
};


