#include "EvppHttpClient.h"
#include <evpp/httpc/conn.h>
#include <future>

CEvppHttpClient::CEvppHttpClient() {
	m_loop.Start(true);
}

CEvppHttpClient::~CEvppHttpClient() {
	m_loop.Stop(true);
}


void CEvppHttpClient::HandleHTTPResponse(const std::shared_ptr<evpp::httpc::Response>& response,
	evpp::httpc::GetRequest* request, std::promise<std::string>& resPromise) {
	delete request; // The request MUST BE deleted in EventLoop thread.
	resPromise.set_value(response->body().ToString());
}

std::string CEvppHttpClient::request(std::string url) {
	evpp::httpc::GetRequest* r = new evpp::httpc::GetRequest(m_loop.loop(), url.c_str(), evpp::Duration(5.0));
	
	std::promise<std::string> resPromise;
	std::future<std::string> resFuture = resPromise.get_future();
	r->Execute(std::bind(&CEvppHttpClient::HandleHTTPResponse, std::placeholders::_1, r, std::ref(resPromise)));
	std::string response = resFuture.get();
	
	return response;
}

