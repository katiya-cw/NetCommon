
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once
#include "vld.h" 

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#import "c:\program files\common files\system\ado\msado15.dll" \
no_namespace \
rename ("EOF", "adoEOF")

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <afxsock.h>            // MFC �׽�����չ


#define RUNEVENTLEN   2048 //��־�ı�����
#define RUNFILELEN    4000 * 1024//��־�ļ���С����

#include <fstream>//
#include <vector>



#ifdef WIN32
#include <process.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "time.h" 
#pragma comment(lib,"Ws2_32.lib")
#define j_min(a,b) min(a,b)

#define SERVER_API extern "C" __declspec(dllexport)

#define socklen_t int

#else

#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>


#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>

#include <sys/time.h>


#include <arpa/inet.h>

#include <sys/socket.h>


typedef long long __int64;
typedef hostent HOSTENT;
typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef unsigned long ULONG;
typedef unsigned char BYTE;
typedef int SOCKET;
typedef linger LINGER;
typedef bool BOOL;
typedef sockaddr_in SOCKADDR_IN;
typedef sockaddr SOCKADDR;
typedef int HANDLE;


#define SERVER_API extern "C"
#define j_min(a,b) (a<b?a:b)
#define MAX_PATH 256
#define TRUE  1
#define FALSE 0
#define closesocket close
#define SD_BOTH SHUT_RDWR

typedef long  HRESULT;


#endif

void SLEEP(unsigned long time);

#include "NetComm.h"

void OutputDebug(const char * fmt, ...);
void OutputDebug2(const char * fmt, ...);

#ifdef WIN32
int receivefrom(SOCKET s,char *pchbuf,int nlen,int nflags,
				struct sockaddr FAR * from,int FAR * fromlen,
				int ntimeoverSec);

int sendtoclient(SOCKET s,char * pchbuf,int nlen,int nflags,
				 const struct sockaddr FAR * to,
				 int ntolen,int ntimeoverSec);


#else

int receivefrom(SOCKET s,char *pchbuf,int nlen,int nflags,
				struct sockaddr *from,int *fromlen,
				int ntimeoverSec);

int sendtoclient(SOCKET s,char * pchbuf,int nlen,int nflags,
				 const struct sockaddr * to,
				 int ntolen,int ntimeoverSec);


#endif

//�������Ϣ��ʾ
#define SERVER_INFO_STRING_MSG   (WM_USER+100)
//�ͻ�����Ϣ��ʾ
#define CLIENT_INFO_STRING_MSG   (WM_USER+101)
//�����URL��Ϣ��ʾ
#define SERVER_URL_STRING_MSG   (WM_USER+102)

//���͵�������б���ʾ
void OutputServerString(CString str);
//���͵��ͻ����б���ʾ
void OutputClientString(CString str);
//���͵�������б���ʾ
void OutputServerURL(CString str);


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif