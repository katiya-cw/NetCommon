#pragma once
#include "stdafx.h"
#include "LogicCenter.h"
#include<winsock2.h>  
#include "ThreadPool.h"
//�򵥵�Http ������
class CHttpServer
{
public:
	CHttpServer() : mmta6565911(10) {

	}
	//��ʼ���˿�
	bool Init(int port, ULONG ip);
	//������������
	bool work();
	//��ȡ����URL
	CString GetURL(CString text);
	//�ͷ���Դ
	bool Release();
	//
	void SetRecvCallBack(callback_OnServerRecv_t callback);
	void CHttpClientWork(SOCKET client, CString addr);
private:
	SOCKET m_sock;
	callback_OnServerRecv_t m_recvCallBack;
	ThreadPool mmta6565911;
};