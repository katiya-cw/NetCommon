// NetComm.h : main header file for the NETCOMM DLL
//

#if !defined(AFX_NETCOMM_H__D261FF71_085E_4B94_8880_DC50DA4973C3__INCLUDED_)
#define AFX_NETCOMM_H__D261FF71_085E_4B94_8880_DC50DA4973C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNetCommApp
// See NetComm.cpp for the implementation of this class
//

class CNetCommApp : public CWinApp
{
public:
	CNetCommApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetCommApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNetCommApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifdef WIN32

#define SERVER_API extern "C" __declspec(dllexport)

#endif

//TCP�Ͽ��ص�����
//
typedef bool (*CALL_TCP_SHUTDOWN)(int nServerSeq,char* pClientIP,int nPort);

//UDPͨ��

///////////////////////////////////////////////////
//����˽ӿ�
///////////////////////////////////////////////////

//����˻ص����������ͻ��˷��͵������ݻص���Ӧ�ó���
//����
//nServerSeq ������� Ψһ����
//pClientIP ������Դ��IP
//nPort ������Դ�Ķ˿�
//pBuff ����
//nLen ���ݳ���
//
typedef bool (*CALL_SERVER_RECEIVE)(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);




///////////////////////////////////////////////////
//�ͻ��˽ӿ�
///////////////////////////////////////////////////

//�ͻ��˻ص����������ͻ��˷��͵������ݻص���Ӧ�ó���

//nClientSeq �ͻ��˺��� Ψһ����
//pServerIP ������Դ��IP
//nPort������Դ�Ķ˿�
//pBuff����
//nLen���ݳ���
//
typedef bool (*CALL_CLIENT_RECEIVE)(int nClientSeq,char* pServerIP,int nPort,char* pBuff,int nLen);


//TCPͨ��
//�ص������ο�UDPͨ��

///////////////////////////////////////////////////
//����˽ӿ�
///////////////////////////////////////////////////

//��ʼ������������һ����ͨ��
//����
//nServerSeq ������룬Ψһ
//nPort ����˿�
//CALL_SERVER_RECEIVE ���ݽ��յĻص�����
//
//����
//true����ʹ�ã�false���󣬴�����Ϣ�鿴��־
#ifndef WIN32
SERVER_API int Tcp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#else
SERVER_API int __stdcall	Tcp_Server_Init(int nServerSeq,int nPort,CALL_SERVER_RECEIVE call_SERVER_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#endif

//����˷��ͺ���
//����
//nServerSeq ������룬��ʼ��ʱ�趨�ĺ���
//pClientIP �ͻ���IP	�ڻص������л�ȡ������ֵ
//nPort �ͻ��˶˿�	�ڻص������л�ȡ������ֵ
//pBuff ��Ҫ���͵�����
//nLen ���ݳ���
//���� ���͵��ֽ���
#ifndef WIN32
SERVER_API int Tcp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#else
SERVER_API int __stdcall	Tcp_Server_Send(int nServerSeq,char* pClientIP,int nPort,char* pBuff,int nLen);
#endif


//�����ֹͣ1����
//����
//nServerSeq ��Ҫֹͣ�ķ�����
//
//����
//true ������false ʧ��
#ifndef WIN32
SERVER_API int Tcp_Server_Release(int nServerSeq);
#else
SERVER_API int __stdcall	Tcp_Server_Release(int nServerSeq);
#endif


///////////////////////////////////////////////////
//�ͻ��˽ӿ�
///////////////////////////////////////////////////

//�ͻ��˻ص����������ͻ��˷��͵������ݻص���Ӧ�ó���


//��ʼ������������һ�ͻ���ͨ��
//����
//nClientSeq �ͻ��˺��룬Ψһ
//pServerIP ������IP
//nPort ����˿�
//CALL_CLIENT_RECEIVE ���ݽ��յĻص�����
//
//����
//true����ʹ�ã�false���󣬴�����Ϣ�鿴��־
#ifndef WIN32
SERVER_API int Tcp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#else
SERVER_API int __stdcall	Tcp_Client_Init(int nClientSeq,char* pServerIP,int nPort,CALL_CLIENT_RECEIVE call_CLIENT_RECEIVE,CALL_TCP_SHUTDOWN call_TCP_SHUTDOWN);
#endif

//�ͻ��˷��ͺ���
//����
//nClientSeq �ͻ��˺��룬��ʼ��ʱ�趨�ĺ���
//pBuff ��Ҫ���͵�����
//nLen ���ݳ���
//���ط��͵��ֽ���


#ifndef WIN32
SERVER_API int Tcp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#else
SERVER_API int __stdcall	Tcp_Client_Send(int nClientSeq,char* pBuff,int nLen);
#endif

//ֹͣ1ͨ��ͨ��
//����
//nClientSeq ��Ҫֹͣ�ı��
//
//����
//true ������false ʧ��
#ifndef WIN32
SERVER_API int Tcp_Client_Release(int nClientSeq);
#else
SERVER_API int __stdcall	Tcp_Client_Release(int nClientSeq);
#endif

//nServerSeq nClientSeq ������ͬ�������е�nServerSeq���������ظ������е�nClientSeq�������ظ�

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETCOMM_H__D261FF71_085E_4B94_8880_DC50DA4973C3__INCLUDED_)
