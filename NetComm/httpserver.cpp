#include "stdafx.h"
#include "httpserver.h"

//��ʼ���˿�
bool CHttpServer::Init(int port,ULONG ip)
{
	
	//CTime tT(t);
	//�����׽���  
	m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == -1)
	{
		OutputServerString("Creat scoket is failed!\n");
		return false;
	}
	
	//OutputDebug("myTEST  time = %s(%d)", tT.Format("%Y-%m-%d %H:%M:%S"), t);
	//���socaddr_in�ṹ  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);//1024--65535�Ķ˿ں�  
	//sin.sin_addr.S_un.S_addr = ip/*INADDR_ANY*/;//ֻ����127.0.0.1�������ܷ��ʣ������޷�����
	sin.sin_addr.S_un.S_addr =/*ip*/INADDR_ANY;  //��������IPҲ�ܷ���
	//������׽��ֵ����ص�ַ  
	if (::bind(m_sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		OutputServerString("bind is failed!\n");
		return false;
	}
	//����ģʽ  
	if (::listen(m_sock, 5) == SOCKET_ERROR)
	{
		OutputServerString("listen is failed!\n");
		return false;
	}
	OutputServerString("���ڼ���....\n");
	m_recvCallBack = NULL;
	return true;
}
//����
bool CHttpServer::work()
{
	//ѭ��������������  
	sockaddr_in addrlink;
	int Len = sizeof(addrlink);
	//char anser[] = "HTTP/1.1 200 OK\r\n\r\n<html><head><title></title></head><body>%s</body></html>\r\n";
	//char buf[1024];
	SOCKET client;
	client = ::accept(m_sock, (SOCKADDR*)&addrlink, &Len);
	OutputDebug("accept one client.");
	if (client == INVALID_SOCKET)
	{
		OutputServerString("accept is failed!");
		return false;
	}
	CString addr;
	addr.Format("%s", ::inet_ntoa(addrlink.sin_addr));
	srand((unsigned)time(NULL));
	mmta6565911.enqueue([](void* httpServer, SOCKET client, CString addr) {
		CHttpServer* phttpServer = (CHttpServer*)httpServer;
		phttpServer->CHttpClientWork(client, addr);
	}, rand(), (void*)this, client, addr);

	return true;
}
void CHttpServer::CHttpClientWork(SOCKET client, CString addr) {
	char anser[] = "HTTP/1.1 200 OK\r\n\r\n<html><head><title></title></head><body>%s</body></html>\r\n";
	char buf[1024];
	int n;
	while (1)
	{
		n = ::recv(client, buf, 1024, 0);
		if (n>0)
		{
			buf[n] = '\0';
			CString strTemp;
			strTemp.Format("�ͻ�:(%s)", addr);
			OutputServerString(strTemp);

			strTemp.Format("\n%s\n", buf);
			OutputServerString(strTemp);
			CString url = GetURL(buf);
			strTemp.Format(anser, url);
			m_recvCallBack(url);
			::send(client, strTemp.GetBuffer(0), strTemp.GetLength(), 0);
			OutputServerURL(url);
		}
		break;
	}
	OutputDebug("finish one client.");
	::closesocket(client);
}
//��ȡ����URL
CString CHttpServer::GetURL(CString text)
{
	CString ret("");
	int getlen = strlen("GET ");
	//����GET
	int first = text.Find("GET ");
	int last  = 0;
	if (first != -1)
	{
		last = text.Find(" ",first + getlen);

		if (last != -1) 
		{
			ret = text.Mid(first+ getlen, last-first- getlen);
		}
	}
	//����HOST
	int hostlen = strlen("Host: ");
	first = text.Find("Host: ");
	if (first != -1)
	{
		last = text.Find("\r\n", first + hostlen);

		if (last != -1)
		{
			ret = text.Mid(first+ hostlen, last- first - hostlen) + ret;
		}
	}

	return ret;
}

//�ͷ���Դ
bool CHttpServer::Release()
{
	if (m_sock == INVALID_SOCKET)
	{
		return false;
	}
	::closesocket(m_sock);
	OutputServerString("ֹͣ������");
	return true;
};

void CHttpServer::SetRecvCallBack(callback_OnServerRecv_t callback)
{
	m_recvCallBack = callback;
}