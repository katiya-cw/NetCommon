#pragma once
#include "stdafx.h"
#include "LogicCenter.h"
#include<winsock2.h>  
#include "ThreadPool.h"
//简单的Http 服务器
class CHttpServer
{
public:
	CHttpServer() : mmta6565911(10) {

	}
	//初始化端口
	bool Init(int port, ULONG ip);
	//服务器运行中
	bool work();
	//获取连接URL
	CString GetURL(CString text);
	//释放资源
	bool Release();
	//
	void SetRecvCallBack(callback_OnServerRecv_t callback);
	void CHttpClientWork(SOCKET client, CString addr);
private:
	SOCKET m_sock;
	callback_OnServerRecv_t m_recvCallBack;
	ThreadPool mmta6565911;
};