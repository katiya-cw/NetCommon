
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
#include "vld.h" 

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#import "c:\program files\common files\system\ado\msado15.dll" \
no_namespace \
rename ("EOF", "adoEOF")

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include <afxsock.h>            // MFC 套接字扩展


#define RUNEVENTLEN   2048 //日志文本长度
#define RUNFILELEN    4000 * 1024//日志文件大小上限

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

//服务端信息显示
#define SERVER_INFO_STRING_MSG   (WM_USER+100)
//客户端信息显示
#define CLIENT_INFO_STRING_MSG   (WM_USER+101)
//服务端URL信息显示
#define SERVER_URL_STRING_MSG   (WM_USER+102)

//发送到服务端列表显示
void OutputServerString(CString str);
//发送到客户端列表显示
void OutputClientString(CString str);
//发送到服务端列表显示
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