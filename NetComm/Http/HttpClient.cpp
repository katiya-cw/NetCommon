#include "HttpClient.h"
#include <evpp/httpc/conn.h>
#include <folly/futures/Future.h>

CHttpClient::CHttpClient() {
	m_loop.Start(true);
}

CHttpClient::~CHttpClient() {
	m_loop.Stop(true);
}

void CHttpClient::HandleHTTPResponse(const std::shared_ptr<evpp::httpc::Response>& response, 
	evpp::httpc::GetRequest* request, folly::Promise<std::string>* resPromise) {
	delete request; // The request MUST BE deleted in EventLoop thread.
	resPromise->setValue(response->body().ToString());
}

std::string CHttpClient::request(std::string url) {
	evpp::httpc::GetRequest* r = new evpp::httpc::GetRequest(m_loop.loop(), url.c_str(), evpp::Duration(5.0));

	folly::Promise<std::string> resPromise;
	folly::Future<std::string> resFuture = resPromise.getFuture();
	r->Execute(std::bind(&CHttpClient::HandleHTTPResponse, std::placeholders::_1, r, &resPromise));
	std::string response = std::move(resFuture).get();

	return response;
}

