#pragma once

#include "stdafx.h"
#include "unzip.h"
#include "Des.h"

CString GetCurrentPathName();
CString GetSystemError(int code);
CString GetTempFileNames();
BOOL FileIsExist(CString name);
BOOL UnZipFile(CString strFile);
CTime ____GetBuildDateTime();
int _ConnectServer(char* ip, int port, SOCKET &s);
char* GetIpByDomain(char* pHost);
void CreateMapFile(char* name, char* pKey, int nKeyLen);
WORD Caluation_CRC16(BYTE Buff[], int nSize);
//获取URL的参数值
std::vector<CString> GetURLParamValue(CString param);
BYTE toHex(const BYTE &x);
CString URLEncode(CString sIn);
CString gettime();
CString gettime2();
