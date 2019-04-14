#pragma once

#include "DbOperate.h"
#include <mutex>

class CDbOperateInstance {
public:
	static CDbOperate* getInstance() {
		static std::mutex lock;
		static CDbOperate* instance = NULL;
		lock.lock();
		if (instance == NULL) {
			instance = new CDbOperate();
		}
		lock.unlock();
		return instance;
	}
};
