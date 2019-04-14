#pragma once

#include "stdafx.h"
#include "ThreadPool.h"
#include <evpp/event_loop.h>
#include <evpp/http/context.h>
#include <evpp/http/http_server.h>
#include <mutex>

class CNetCommHttpServerInstance {
public:
	static CNetCommHttpServerInstance* getInstance() {
		static CNetCommHttpServerInstance* instance = NULL;
		static std::mutex lock;
		lock.lock();
		if (instance == NULL) {
			instance = new CNetCommHttpServerInstance();
		}
		lock.unlock();
		return instance;
	}

public:
	bool start(int threadNum, int port);

private:
	void handler(evpp::EventLoop* loop,
		const evpp::http::ContextPtr& ctx,
		const evpp::http::HTTPSendResponseCallback& cb);

private:
	ThreadPool m_threadPool;
};
