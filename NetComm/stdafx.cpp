
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// NetComm.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include "evpp\logging.h"

extern HANDLE g_hOut;

void OutputDebug(const char * fmt, ...)
{
    char szData[512]={0};
	
    char data[512]={0};
    va_list args;
	va_start(args, fmt);
	_vsnprintf_s(szData, sizeof(szData) - 1, fmt, args);
	va_end(args);
	
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	
	sprintf_s(data,"%02d:%02d:%02d %s\n",sys.wHour,sys.wMinute,sys.wSecond,szData);
	LOG_INFO << data;
	OutputDebugString(data);
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
	LOG_INFO << data;
}
//���͵�������б���ʾ
void OutputServerString(CString str)
{
	LOG(INFO) << "output server string : " << str;
	CWnd* pHwnd = AfxGetApp()->GetMainWnd();
	if (pHwnd) 
	{
		::SendMessage(pHwnd->m_hWnd, SERVER_INFO_STRING_MSG, (WPARAM)str.GetBuffer(0), NULL);
	}
}

//���͵��ͻ����б���ʾ
void OutputClientString(CString str)
{
	LOG(INFO) << "output client string : " << str;
	CWnd* pHwnd = AfxGetApp()->GetMainWnd();
	if (pHwnd)
	{
		::SendMessage(pHwnd->m_hWnd, CLIENT_INFO_STRING_MSG, (WPARAM)str.GetBuffer(0), NULL);
	}
}

//���͵�������б���ʾ
void OutputServerURL(CString str)
{
	LOG(INFO) << str.GetBuffer();
	CWnd* pHwnd = AfxGetApp()->GetMainWnd();
	if (pHwnd)
	{
		::SendMessage(pHwnd->m_hWnd, SERVER_URL_STRING_MSG, (WPARAM)str.GetBuffer(0), NULL);
	}
}