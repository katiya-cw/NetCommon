
// stdafx.cpp : 只包括标准包含文件的源文件
// NetComm.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

#define ELPP_NO_DEFAULT_LOG_FILE
#include "LogUtil\easylogging++.h"
INITIALIZE_EASYLOGGINGPP;

extern HANDLE g_hOut;

void OutputDebug(const char * fmt, ...)
{
    char szData[512]={0};
	
#ifdef WIN32	
    char data[512]={0};
    va_list args;
	va_start(args, fmt);
	_vsnprintf_s(szData, sizeof(szData) - 1, fmt, args);
	va_end(args);
	
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	
	sprintf_s(data,"%02d:%02d:%02d %s\n",sys.wHour,sys.wMinute,sys.wSecond,szData);
	LOG(INFO) << data;
	OutputDebugString(data);
#else
    va_list args;
    int n;
    va_start(args, fmt);
    n = vsprintf(szData, fmt, args);
    va_end(args);
	
	time_t now = time(0);
	tm *tnow = localtime(&now);
	
	printf("%02d:%02d:%02d %s\n",tnow->tm_hour,tnow->tm_min,tnow->tm_sec,szData);
#endif
}
void OutputDebug2(const char * fmt, ...)
{
	

	char szData[512] = { 0 };
	char data[512] = { 0 };
	memset(data, 0, sizeof(data));

	va_list args;
	va_start(args, fmt);
	_vsnprintf(szData, sizeof(szData) - 1, fmt, args);
	va_end(args);

	SYSTEMTIME sys;
	GetLocalTime(&sys);

	sprintf(data, "%s %s\n", CTime::GetCurrentTime().Format("%H:%M:%S"), szData);
	//sys.wMinute,
	OutputDebugString(data);
	LOG(INFO) << data;
	//if (g_hOut != 0)
	{
		_tprintf(_T("%s"), data);
	}
}
//发送到服务端列表显示
void OutputServerString(CString str)
{
	LOG(INFO) << "output server string : " << str;
	CWnd* pHwnd = AfxGetApp()->GetMainWnd();
	if (pHwnd) 
	{
		::SendMessage(pHwnd->m_hWnd, SERVER_INFO_STRING_MSG, (WPARAM)str.GetBuffer(0), NULL);
	}
}

//发送到客户端列表显示
void OutputClientString(CString str)
{
	LOG(INFO) << "output client string : " << str;
	CWnd* pHwnd = AfxGetApp()->GetMainWnd();
	if (pHwnd)
	{
		::SendMessage(pHwnd->m_hWnd, CLIENT_INFO_STRING_MSG, (WPARAM)str.GetBuffer(0), NULL);
	}
}

//发送到服务端列表显示
void OutputServerURL(CString str)
{
	LOG(INFO) << str.GetBuffer();
	CWnd* pHwnd = AfxGetApp()->GetMainWnd();
	if (pHwnd)
	{
		::SendMessage(pHwnd->m_hWnd, SERVER_URL_STRING_MSG, (WPARAM)str.GetBuffer(0), NULL);
	}
}