#include "stdafx.h"
#include "httpserver.h"

//初始化端口
bool CHttpServer::Init(int port,ULONG ip)
{
	
	//CTime tT(t);
	//创建套接字  
	m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == -1)
	{
		OutputServerString("Creat scoket is failed!\n");
		return false;
	}
	
	//OutputDebug("myTEST  time = %s(%d)", tT.Format("%Y-%m-%d %H:%M:%S"), t);
	//填充socaddr_in结构  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);//1024--65535的端口号  
	//sin.sin_addr.S_un.S_addr = ip/*INADDR_ANY*/;//只能是127.0.0.1本机才能访问，外网无法访问
	sin.sin_addr.S_un.S_addr =/*ip*/INADDR_ANY;  //任意外网IP也能访问
	//绑定这个套接字到本地地址  
	if (::bind(m_sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		OutputServerString("bind is failed!\n");
		return false;
	}
	//监听模式  
	if (::listen(m_sock, 5) == SOCKET_ERROR)
	{
		OutputServerString("listen is failed!\n");
		return false;
	}
	OutputServerString("正在监听....\n");
	m_recvCallBack = NULL;
	return true;
}
//运行
bool CHttpServer::work()
{
	//循环接受连接请求  
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
			strTemp.Format("客户:(%s)", addr);
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
//获取连接URL
CString CHttpServer::GetURL(CString text)
{
	CString ret("");
	int getlen = strlen("GET ");
	//查找GET
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
	//查找HOST
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

//释放资源
bool CHttpServer::Release()
{
	if (m_sock == INVALID_SOCKET)
	{
		return false;
	}
	::closesocket(m_sock);
	OutputServerString("停止服务器");
	return true;
};

void CHttpServer::SetRecvCallBack(callback_OnServerRecv_t callback)
{
	m_recvCallBack = callback;
}