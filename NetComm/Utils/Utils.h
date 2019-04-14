#pragma once

#include "stdafx.h"
#include "unzip.h"
#include "Des.h"

inline CString GetCurrentPathName()
{
	char buf[MAX_PATH] = { 0 };
	DWORD len = MAX_PATH;

	GetModuleFileName(NULL, buf, len);
	CString exe;
	exe.Format("%s", buf);
	int n = exe.ReverseFind('\\');
	exe = exe.Left(n);

	return exe;
}

inline CString GetSystemError(int code)
{
	CString strR;
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);
	strR.Format("%s", lpMsgBuf);

	return strR;

}

inline CString GetTempFileNames()
{
	CString strName;
	char szTempName[MAX_PATH];

	TCHAR   szPathCopy[_MAX_PATH];
	GetTempPath(_MAX_PATH, szPathCopy);
	GetTempFileName(szPathCopy, // dir. for temp. files 
		"NEW",                // temp. file name prefix 
		0,                    // create unique name 
		szTempName);          // buffer for name 

	strName.Format("%s", szTempName);
	return strName;
}

inline BOOL FileIsExist(CString name)
{
	CStdioFile file;
	if (file.Open(name, CFile::modeRead))
	{
		file.Close();
		return TRUE;
	}
	return FALSE;
}

inline BOOL UnZipFile(CString strFile)
{
	char strFileName[MAX_PATH] = { 0 };
	strcpy_s(strFileName, strFile);

	char path[MAX_PATH] = { 0 };

	int n = strFile.ReverseFind('\\');
	sprintf_s(path, "%s", strFile.Left(n).GetBuffer(0));
	//	SetCurrentDirectory(path);

	char strDestName[MAX_PATH] = { 0 };
	HZIP hz = OpenZip(strFileName, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	for (int i = 0; i<numitems; i++)
	{
		GetZipItem(hz, i, &ze);
		sprintf_s(strDestName, "%s\\%s", path, ze.name);

		UnzipItem(hz, i, strDestName);
	}

	CloseZip(hz);

	return TRUE;
}

CTime ____GetBuildDateTime()
{
	char s_month[5];
	int month, day, year;
	char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
	sscanf(__DATE__, "%s %d %d", s_month, &day, &year);
	month = (strstr(month_names, s_month) - month_names) / 3 + 1;

	int h, m, s;
	sscanf(__TIME__, "%d:%d:%d", &h, &m, &s);

	CTime t(year, month, day, h, m, s);

	return t;
}

int _ConnectServer(char* ip, int port, SOCKET &s)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	WSAStartup(wVersionRequested, &wsaData);

	char strError[1024] = { 0 };
	sockaddr_in srv;
	memset(&srv, 0, sizeof(srv));
	srv.sin_addr.S_un.S_addr = inet_addr(ip);
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s <= 0)
	{
		int error = GetLastError();
		sprintf(strError, "socket error %d (%s) File:%s Line = %d ", error, GetSystemError(error), __FILE__, __LINE__);
		OutputDebug(strError);
		return 1;
	}
	sockaddr_in sin;
#ifndef WIN32
	sin.sin_addr.s_addr = INADDR_ANY;
#else
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
#endif
	sin.sin_family = AF_INET;
	sin.sin_port = htons(0);

	if (bind(s, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		closesocket(s);
		int error = GetLastError();
		sprintf(strError, "bind error  %d (%s) File:%s Line = %d ", error, GetSystemError(error), __FILE__, __LINE__);
		OutputDebug(strError);
		return 2;
	}
	if (connect(s, (struct sockaddr*)&srv, sizeof(srv)) != 0)
	{
		int error = GetLastError();
		closesocket(s);

		sprintf(strError, "Connect error %d  File:%s Line = %d ", error, __FILE__, __LINE__);
		OutputDebug(strError);

		return 3;
	}
	return 0;
}

char* GetIpByDomain(char* pHost)//ÓòÃû ×ªipµØÖ·
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	WSAStartup(wVersionRequested, &wsaData);

	static char ipv4[16] = { 0 };
	if (inet_addr(pHost) != INADDR_NONE)
	{
		strcpy(ipv4, pHost);
	}
	else
	{
		hostent* answer = gethostbyname(pHost);
		if (answer != NULL)
		{
			BYTE ip[4] = { 0 };
			memcpy(ip, answer->h_addr_list[0], 4);

			sprintf(ipv4, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
		}
	}
	return ipv4;
}

void CreateMapFile(char* name, char* pKey, int nKeyLen)
{
	char str[21][200] =
	{
		"<!DOCTYPE html>\r\n",
		"<html>\r\n",
		"<head>\r\n",
		"<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n",
		"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n",
		"<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\" />\r\n",
		"<style type=\"text/css\">\r\n",
		"body, html,#allmap {width: 100%;height: 100%;overflow: hidden;margin:0;}\r\n",
		"</style>\r\n",
		"<script type=\"text/javascript\" src=\"http://api.map.baidu.com/api?v=2.0&ak=",//................
		"\"></script>\r\n",
		"<script type=\"text/javascript\" src=\"TextIconOverlay.js\"></script>\r\n",
		"<script type=\"text/javascript\" src=\"MarkerClusterer.js\"></script>\r\n",
		"<title>map</title>\r\n",
		"</head>\r\n",
		"<body>\r\n",
		"<div id=\"allmap\"></div>\r\n",
		"<script type=\"text/javascript\" src=\"MapInit.js\"></script>\r\n",
		"<script type=\"text/javascript\" src=\"MapHelper.js\"></script>\r\n",
		"</body>\r\n",
		"</html>\r\n"
	};

	CStdioFile file;
	if (!file.Open(name, CFile::modeWrite | CFile::modeCreate))
	{
		return;
	}
	strcat(str[9], pKey);

	for (int i = 0;i < 21;i++)
	{
		file.Write(str[i], strlen(str[i]));
	}
	file.Close();

}

WORD Caluation_CRC16(BYTE Buff[], int nSize)
{
	WORD nOld;
	WORD nRet = 0xffff;
	unsigned short i, j;
	for (i = 0; i<nSize; i++)
	{
		nRet ^= Buff[i];
		for (j = 0; j<8; j++)
		{
			nOld = nRet;
			nRet >>= 1;
			if (nOld & 0x0001)
				nRet ^= 0xa001;
		}
	}
	return nRet;
}
