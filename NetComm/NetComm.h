
// NetComm.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CNetCommApp: 
// 有关此类的实现，请参阅 NetComm.cpp
//

class CNetCommApp : public CWinApp
{
public:
	CNetCommApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CNetCommApp theApp;

//TCP断开回调函数
//
typedef bool (*CALL_TCP_SHUTDOWN)(int nServerSeq,char* pClientIP,int nPort);

//UDP通信

///////////////////////////////////////////////////
//服务端接口
///////////////////////////////////////////////////

//服务端回调函数，将客户端发送到的数据回调给应用程序
//参数
//nServerSeq 服务号码 唯一的数
//pClientIP 数据来源的IP
//nPort 数据来源的端口
//pBuff 数据
//nLen 数据长度
//
typedef bool (*CALL_SERVER_RECEIVE)(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);




///////////////////////////////////////////////////
//客户端接口
///////////////////////////////////////////////////

//客户端回调函数，将客户端发送到的数据回调给应用程序

//nClientSeq 客户端号码 唯一的数
//pServerIP 数据来源的IP
//nPort数据来源的端口
//pBuff数据
//nLen数据长度
//
typedef bool (*CALL_CLIENT_RECEIVE)(int nClientSeq,char* pServerIP,int nPort,char* pBuff,int nLen);


//TCP通信
//回调函数参考UDP通信

///////////////////////////////////////////////////
//服务端接口
///////////////////////////////////////////////////

//初始化函数，创建一服务通道
//参数
//nServerSeq 服务号码，唯一
//nPort 服务端口
//CALL_SERVER_RECEIVE 数据接收的回调函数
//
//返回
//true可以使用，false错误，错误信息查看日志
#ifndef WIN32
SERVER_API int Tcp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#else
SERVER_API int __stdcall	Tcp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#endif

//服务端发送函数
//参数
//nServerSeq 服务号码，初始化时设定的号码
//pClientIP 客户端IP	在回调函数中获取到的数值
//nPort 客户端端口	在回调函数中获取到的数值
//pBuff 需要发送的数据
//nLen 数据长度
//返回 发送的字节数
#ifndef WIN32
SERVER_API int Tcp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#else
SERVER_API int __stdcall	Tcp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#endif


//服务端停止1服务
//参数
//nServerSeq 需要停止的服务编号
//
//返回
//true 结束，false 失败
#ifndef WIN32
SERVER_API int Tcp_Server_Release(int nServerSeq);
#else
SERVER_API int __stdcall	Tcp_Server_Release(int nServerSeq);
#endif


///////////////////////////////////////////////////
//客户端接口
///////////////////////////////////////////////////

//客户端回调函数，将客户端发送到的数据回调给应用程序


//初始化函数，创建一客户端通道
//参数
//nClientSeq 客户端号码，唯一
//pServerIP 服务器IP
//nPort 服务端口
//CALL_CLIENT_RECEIVE 数据接收的回调函数
//
//返回
//true可以使用，false错误，错误信息查看日志
#ifndef WIN32
SERVER_API int Tcp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#else
SERVER_API int __stdcall	Tcp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#endif

//客户端发送函数
//参数
//nClientSeq 客户端号码，初始化时设定的号码
//pBuff 需要发送的数据
//nLen 数据长度
//返回发送的字节数


#ifndef WIN32
SERVER_API int Tcp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#else
SERVER_API int __stdcall	Tcp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#endif

//停止1通道通信
//参数
//nClientSeq 需要停止的编号
//
//返回
//true 结束，false 失败
#ifndef WIN32
SERVER_API int Tcp_Client_Release(int nClientSeq);
#else
SERVER_API int __stdcall	Tcp_Client_Release(int nClientSeq);
#endif

//nServerSeq nClientSeq 可以相同，但所有的nServerSeq，不可以重复，所有的nClientSeq不可以重复