#include "HttpClient.h"
#include <evpp/httpc/conn.h>
#include <future>

CHttpClient::CHttpClient() {
	m_loop.Start(true);
}

CHttpClient::~CHttpClient() {
	m_loop.Stop(true);
}


void CHttpClient::HandleHTTPResponse(const std::shared_ptr<evpp::httpc::Response>& response, 
	evpp::httpc::GetRequest* request, std::promise<std::string>& resPromise) {
	delete request; // The request MUST BE deleted in EventLoop thread.
	resPromise.set_value(response->body().ToString());
}

std::string CHttpClient::request(std::string url) {
	evpp::httpc::GetRequest* r = new evpp::httpc::GetRequest(m_loop.loop(), url.c_str(), evpp::Duration(5.0));
	
	std::promise<std::string> resPromise;
	std::future<std::string> resFuture = resPromise.get_future();
	r->Execute(std::bind(&CHttpClient::HandleHTTPResponse, std::placeholders::_1, r, std::ref(resPromise)));
	std::string response = resFuture.get();
	
	return response;
}

