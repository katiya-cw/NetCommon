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

