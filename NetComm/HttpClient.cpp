#include "stdafx.h"
#include "httpclient.h"
#include "../../MyDefine.h"
/*************************类实现********************************/
CHttpClient::CHttpClient()
{

}

 CHttpClient::~CHttpClient()
{

}
// 添加字段  
void   CHttpClient::addParam(CString   name, CString   value)
{
	names.AddTail((LPCTSTR)name);
	values.AddTail((LPCTSTR)value);
	CString   eq = "=";
	CString   an = "&";
	CONTENT = CONTENT + name + eq + value + an;
	CL = CONTENT.GetLength();
}

void   CHttpClient::addParam(CString param)
{

}
// 以http Get方式请求URL  
CString   CHttpClient::doGet(CString   href)
{
	CString   httpsource = "";
	CInternetSession   session1(NULL, 0);
	CHttpFile*   pHTTPFile = NULL;
	try {
#ifdef outdebugGetText
		pHTTPFile = (CHttpFile*)session1.OpenURL(href, 1, INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD, NULL, 0);//每次读取都清缓存区
#else		
		pHTTPFile = (CHttpFile*)session1.OpenURL(href);
#endif
	}
	catch (CInternetException *ex)
	{
		CString strInfo;
		strInfo.Format(_T("CInternetException ErrorCode = %ld"), ex->m_dwError);
		pHTTPFile = NULL;
		// 需要 close session
		session1.Close();
		ex->Delete();
	}
	if (pHTTPFile)
	{
		CString   text;
		for (int i = 0; pHTTPFile->ReadString(text); i++)
		{
			httpsource = httpsource + text + "\r\n";
		}
#ifdef outdebugGetText
		OutputClientString("<Information returned by 'get'> :"+ httpsource);//发给发送窗口作为提示
#endif
		// 需要 close session
		session1.Close();
		pHTTPFile->Close();
		delete pHTTPFile;
	}
	else
	{

	}
	return   httpsource;
}
// 以Http Post方式请求URL  
CString   CHttpClient::doPost(CString   href)
{
	CString   httpsource = "";
	CInternetSession   session1;
	CHttpConnection*   conn1 = NULL;
	CHttpFile*   pFile = NULL;
	CString   strServerName;
	CString   strObject;
	INTERNET_PORT   nPort;
	DWORD   dwServiceType;
	AfxParseURL((LPCTSTR)href, dwServiceType, strServerName, strObject, nPort);
	DWORD   retcode;
	char*   outBuff = CONTENT.GetBuffer(1000);
	try
	{
		conn1 = session1.GetHttpConnection(strServerName, nPort);
		pFile = conn1->OpenRequest(0, strObject, NULL, 1, NULL, "HTTP/1.1", INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT);
		pFile->AddRequestHeaders("Content-Type:   application/x-www-form-urlencoded");
		pFile->AddRequestHeaders("Accept:   */*");
		pFile->SendRequest(NULL, 0, outBuff, strlen(outBuff) + 1);
		pFile->QueryInfoStatusCode(retcode);
	}
	catch (CInternetException   *   e) {
		session1.Close();
		pFile = NULL;
	};
	if (pFile)
	{
		CString   text;
		for (int i = 0; pFile->ReadString(text); i++)
		{
			httpsource = httpsource + text + "\r\n";
		}
		session1.Close();
		pFile->Close();
		delete conn1;
		delete pFile;
	}
	else
	{
		// do anything.....  
	}
	return   httpsource;
	//delete   pFile;
	//delete   conn1;
	//session1.Close();
}