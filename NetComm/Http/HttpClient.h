#pragma once

#include "stdafx.h"

#include <string>
#include <evpp/event_loop_thread.h>
#include <evpp/httpc/request.h>
#include <evpp/httpc/response.h>


class CHttpClient {
public:
	CHttpClient();
	~CHttpClient();

public:
	std::string request(std::string url);

public:
	/*
	static void HandleHTTPResponse(const std::shared_ptr<evpp::httpc::Response>& response, evpp::httpc::GetRequest* request, 
		folly::Promise<std::string>* resPromise);
	*/

private:
  evpp::EventLoopThread m_loop;	
};

