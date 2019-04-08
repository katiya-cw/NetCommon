#pragma once

#include "ConnectBase.h"

class MyConnectionA : public ConnectBase {
public:
	MyConnectionA(evpp::TCPConnPtr tcpConn) : ConnectBase(tcpConn) {}

	void onMsg(evpp::Buffer* msg) {
		if (msg == nullptr || msg->length() <= 0) {
			return;
		}
		size_t dataLen = msg->length();
		onRead(msg->data(), dataLen);
	}
	
private:
	void onRead(const char* data, const size_t& numBytes);
};