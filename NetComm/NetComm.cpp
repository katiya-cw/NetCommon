
// NetComm.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "NetComm.h"
#include "NetCommDlg.h"
#include "unzip.h"
#include "Des.h"
#include "../MyDefine.h"
#include "DialogReg.h"
#include "db/DbOperate.h"
#include "db/DbOperateInstance.h"
#include "Utils/Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int g_nThisVer = 4;//�汾��

int tcpreceive(SOCKET s,char *pchbuf,int nsize,int ntimeoverSec)
{
	int   status,nbytesreceived;
	if(s==-1)
	{
		return  -1;
	}
	struct   timeval   tv={0,ntimeoverSec};
	fd_set   fd;
	FD_ZERO(&fd);
	FD_SET(s,&fd);
	if(ntimeoverSec==0)
	{
		status=select(s+1,&fd,(fd_set   *)NULL,(fd_set   *)NULL,NULL);
	}
	else
	{
		status=select(s+1,&fd,(fd_set   *)NULL,(fd_set   *)NULL,&tv);
	}
	switch(status)
	{
	case   -1:
		//printf("read   select   error\n");
		return   -1;
	case   0:
		//printf("receive   time   out\n");
		return   0;
	default:
		if(FD_ISSET(s,&fd))
		{
			if((nbytesreceived=recv(s,pchbuf,nsize,0))==-1)
			{
					int kkk=0;
					kkk=WSAGetLastError();
				return   0;
			}
			else
			{
				return   nbytesreceived;
			}
		}
	}
	return 0;
}

BOOL GetModelFromRes(char* strFileName)
{
	return TRUE;
}

char g_strPIP[32] = {0};
int g_nPort = 0;

// CNetCommApp

BEGIN_MESSAGE_MAP(CNetCommApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNetCommApp ����

CNetCommApp::CNetCommApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNetCommApp ����

CNetCommApp theApp;


// CNetCommApp ��ʼ��

BOOL CNetCommApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	WSADATA swadata;
	WORD sockVersion = MAKEWORD(2, 0);//winsock�İ汾2.0  
	WSAStartup(sockVersion, &swadata);//����WInsock�⣬swadata���ؿ����Ϣ


	CoInitialize(NULL);

	AfxInitRichEdit();
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CNetCommDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	::WSACleanup();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


#include "DLL.h"


typedef struct _GET_DATA_ 
{ 
	CALL_BACK_GET_DATA callGetBack;
	CALL_BACK_EXCUTE callExcuteBack;
	CALL_BACK_SHUTDOWN callShutdownBack;

	CALL_BACK_GET_DATA_GPRS callGetBack2;

	SOCKET sSocket;
	BOOL bConnect;
	
}_GET_DATA_;


typedef struct _DOWNLOAD_DATA_ 
{ 
	char name[MAX_PATH];
	CALL_BACK_DOWNLOAD callBack;
	SOCKET sSocket;
	
}_DOWNLOAD_DATA_;

UINT ______AFX_DOWN_LOAD(LPVOID pParam)
{
	_DOWNLOAD_DATA_* pData = (_DOWNLOAD_DATA_* )pParam;

	char strMapFile[MAX_PATH] = {0};
	sprintf(strMapFile,"%s\\js\\map.html",GetCurrentPathName());
	DeleteFile(strMapFile);

	char name[MAX_PATH] = {0};
	strcpy(name,pData->name);

	CALL_BACK_DOWNLOAD callBack = pData->callBack;

	char strError[1024] = {0};

	sprintf(strError,"���ӳɹ����汾��֤ %d",g_nThisVer);
	callBack(5,strError,strlen(strError));

	int nType = 100;
	char buff[1024 * 100] = {0};
	memcpy(&buff[0],&nType,4);
	memcpy(&buff[4],&g_nThisVer,4);
	
	int n = send(pData->sSocket,buff,8,0);//�汾
	if(n < 0)
	{
		int d = GetLastError();
		closesocket(pData->sSocket);

		sprintf(strError,"send error File:%s Line = %d ",__FILE__,__LINE__);
		callBack(1,strError,strlen(strError));
		delete pData;
		return 0;
	}

	OutputDebug("���ͱ��ذ汾 %d",g_nThisVer);
	DWORD dwStart = GetTickCount();
	DWORD dwEnd = GetTickCount();
	BOOL bTimeOut = TRUE;
	//check ver
	while(dwEnd - dwStart < 60 * 1000)
	{
		dwEnd = GetTickCount();
		int nLen = tcpreceive(pData->sSocket,(char* )buff,8,100);

		if(nLen > 0)
		{
			int ver = 0;
			memcpy(&nType,&buff[0],4);
			memcpy(&ver,&buff[4],4);
		}
		if(nLen == 8)
		{
			int ver = 0;
			memcpy(&nType,&buff[0],4);
			memcpy(&ver,&buff[4],4);

			if(ver != g_nThisVer || nType != 100)
			{
				closesocket(pData->sSocket);

				sprintf(strError,"ver check error server %d local %d File:%s Line = %d ",ver,g_nThisVer,__FILE__,__LINE__);
				callBack(3,strError,strlen(strError));

				delete pData;
				return 0;
			}
			else
			{
				nLen = tcpreceive(pData->sSocket,(char* )buff,4,100);
				if(nLen == 4)
				{
					int nKeyLen = 0;
					memcpy(&nKeyLen,&buff[0],4);
					nLen = tcpreceive(pData->sSocket,(char* )buff,nKeyLen,100);
					
					if(nLen == nKeyLen)
					{
						CreateMapFile(strMapFile,buff,nKeyLen);
					}
				}

			}
			bTimeOut = FALSE;
			break;
		}
		Sleep(100);
	}
	if(bTimeOut)
	{
		closesocket(pData->sSocket);

		sprintf(strError,"�汾��֤��ʱ File:%s Line = %d ",__FILE__,__LINE__);
		callBack(9,strError,strlen(strError));
		delete pData;
		return 0;
	}

	//start dwonload
	Sleep(100);
	bTimeOut = TRUE;
	dwStart = GetTickCount();
	dwEnd = GetTickCount();

	int nFileLen = 0;//file size
	while(dwEnd - dwStart < 60 * 1000)
	{
		dwEnd = GetTickCount();
		int nLen = tcpreceive(pData->sSocket,(char* )buff,4,100);

		if(nLen == 4)
		{
			bTimeOut = FALSE;

			memcpy(&nFileLen,&buff[0],4);

			OutputDebug("�ļ���С = %d\n\n\n",nFileLen);
			sprintf(strError,"��ʼ�����ļ�,�ļ���С %d",nFileLen);
			callBack(8,strError,strlen(strError));

			break;
		}
		Sleep(100);
	}
	if(bTimeOut)
	{
		closesocket(pData->sSocket);

		sprintf(strError,"check ver timeout File:%s Line = %d ",__FILE__,__LINE__);
		callBack(9,strError,strlen(strError));
		delete pData;
		return 0;
	}

	CStdioFile file;
	char strT[MAX_PATH] = {0};
	strcpy(strT,GetTempFileNames());

	if(!file.Open(strT,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		closesocket(pData->sSocket);
		int error = GetLastError();

		sprintf(strError,"create tem file error  %d (%s) File:%s Line = %d ",error,GetSystemError(error),__FILE__,__LINE__);
		callBack(1,strError,strlen(strError));

	}
	int nDownLoadSize = 0;
	dwStart = GetTickCount();
	dwEnd = GetTickCount();

	while(dwEnd - dwStart < 180 * 1000)
	{
		dwEnd = GetTickCount();
		int nLen = tcpreceive(pData->sSocket,(char* )buff,1024 * 100,100);

		if(nLen > 0)
		{
			nDownLoadSize += nLen;

			file.Write(buff,nLen);

			sprintf(strError,"�ļ����ؽ��� %.2f %%",nDownLoadSize * 100.0f / nFileLen);
			callBack(8,strError,strlen(strError));

			if(nDownLoadSize >= nFileLen)
			{
				file.Close();

				if(FileIsExist(name))
				{
					DeleteFile(name);
				}


				char tempZip[MAX_PATH] = {0};
				sprintf(tempZip,"%s.zip",name);

				CopyFile(strT,tempZip,FALSE);
				UnZipFile(tempZip);
				char tem[MAX_PATH] = {0};
				memcpy(tem,tempZip,strlen(tempZip) - 3);
				strcat(tem,"tem");
				CopyFile(tem,name,FALSE);

				DeleteFile(strT);
				DeleteFile(tempZip);
				DeleteFile(tem);

				strcpy(strError,"���ݿ��������. ");
				closesocket(pData->sSocket);

				callBack(0,strError,strlen(strError));
				delete pData;
				return 0;
			}
		}
	}

	delete pData;
	return 0;
}

void MaskRem(CString strKey,CString strData,CString & strNew)
{
	CDES des;
	des.InitializeKey(strKey,TRUE);
	des.DecryptAnyLength(strData,TRUE,strNew);
}

int ___CheckAuth()
{
	CString strKey;
	char buff[MAX_PATH] = {0};
	GetSystemDirectory(buff,MAX_PATH);
	
	char path[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH,path);
	
	if(strlen(buff) > 3 && strlen(path) > 3)
	{
		CString s1(buff);
		
		DWORD ID1 = GetDiskSerial(s1.Left(3));
		
		if(ID1 > 0)
		{
			strKey.Format("%X",ID1 * 3);
		}
	}
	
	char sys[MAX_PATH] = {0};
	GetSystemDirectory(sys,MAX_PATH);
	
	CString strFile;
	strFile.Format("%s\\s1n1.dat",sys);//Read reg
	
	CStdioFile file;
	if(file.Open(strFile,CFile::modeRead|CFile::typeBinary))
	{
		char buff[1024] = {0};
		UINT u = file.Read(buff,1024);
		
		if(u > 0 && u < 40)
		{
			CString strSn;
			MaskRem(strKey,buff,strSn);
			
			if(strSn.GetLength() != 8)
			{
			//	AfxMessageBox("ע�������.");
				return FALSE;
			}
			CString strY,strM,strD;
			
			strY = strSn.Left(4);
			strM = strSn.Mid(4,2);
			strD = strSn.Mid(6,2);
			
			int y = atoi(strY),m = atoi(strM),d = atoi(strD);
			if(y == 1 && m == 2 && d == 4)
			{
				return 0;
				//m_edDay.SetWindowText("�����ư汾");
			}
			else
			{
				SYSTEMTIME sys;
				GetLocalTime(&sys);

				int nEnd = y * 10000 + m * 100 + d;

				int nCurr = sys.wYear * 10000 + sys.wMonth * 100 + sys.wDay;
				if(nEnd >= nCurr)
				{
					return 0;
				}
				else
				{
					AfxMessageBox("��Ȩ�Ѿ�����.");

					return 1;//��Ȩ�Ѿ�����
				}
			}
			
		}
	}
	return -1;//��ע����
}

int pascal regMYdevice(void)
{
	int nAuth = ___CheckAuth();
	if(nAuth != 0)
	{
		CDialogReg dlg;
		if(nAuth == -1)
			dlg.m_bNoSn = TRUE;
		dlg.DoModal();
		return 0;
	}
	return 1;
}

int pascal DownLoadFile(char* pServerIP,int nPort,char* strSaveFile,CALL_BACK_DOWNLOAD callBack)
{
	int nAuth = ___CheckAuth();
	if(nAuth != 0)
	{
		CDialogReg dlg;
		if(nAuth == -1)
			dlg.m_bNoSn = TRUE;
		dlg.DoModal();
		return 0;
	}

	char ip[32] = {0};
	strcpy(ip,GetIpByDomain(pServerIP));
	
	_DOWNLOAD_DATA_* pData = new _DOWNLOAD_DATA_;

	pData->callBack = callBack;
	strcpy(pData->name,strSaveFile);
	
	int nRet = _ConnectServer(ip,nPort,pData->sSocket);//down
	if(nRet != 0)
	{
		delete pData;
		return nRet; 
	}

	AfxBeginThread(______AFX_DOWN_LOAD,pData);

	return 0;
}


CDbOperate SqlOPP2;

_GET_DATA_ g_GetData;
UINT ______AFX_CHECK_METER(LPVOID pParam)
{
	int nRet = 0;

	CALL_BACK_GET_DATA callGetBack = g_GetData.callGetBack;
	CALL_BACK_EXCUTE callExcuteBack = g_GetData.callExcuteBack;
	CALL_BACK_GET_DATA_GPRS callGetBack2 = g_GetData.callGetBack2;

	::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
	SqlOPP2.OpenSql();
START:
	char strError[1024] = {0};

	g_GetData.bConnect = TRUE;
	char strS[32] = {0};
	strS[0] = 0x9;
	strS[1] = 0x9;
	strS[2] = 0x9;
	strS[3] = 0x9;

	DWORD dwLastRecv = 0;
	dwLastRecv = 0;//GetTickCount();
	while(TRUE)
	{
		if(GetTickCount() - dwLastRecv > 30 * 1000) //���ڲ�ѯ�������ϵ�ʱ�䣬�����Լ���ʱ�� �൱���Ǹ����������ṩ����
		{
			dwLastRecv = GetTickCount();

		//	if(send(g_GetData.sSocket,strS,1,0) <= 0)
		    if (send(g_GetData.sSocket, strS, 4, 0) <= 0) //Ϊ�����Ϸ���˰�4���ֽ��ж����ͣ�������ⷢ4���ֽ�ʱ���������
			{
				break;
			}
			OutputDebug("��ѯ.....");

		}
		char buff[1024] = {0};
		int nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,100);//����

		if(nLen > 0)
		{
			OutputDebug("%d  %s",nLen,buff);
		}
		if(nLen == -1)//�Ͽ�
		{
			break;
		}
		else if(nLen == 4)
		{
			int nType = 0;
			memcpy(&nType,&buff[0],4);

			if(nType == 0x67)//GetData dtu  //�����������ݰ����ֽڷ����˸ı䣬��0X68��Ϊ0X67������λ�����մ���
			{
				nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,900);//����
				if(nLen == 4)
				{
					int len = 0;
					memcpy(&len,&buff[0],4);
					
					if(len == 12)
					{
						nLen = tcpreceive(g_GetData.sSocket,(char* )buff,12,900);
						if(nLen == 12)
						{
							int ID,status;
							long t;
							memcpy(&ID,&buff[0],4);
							memcpy(&status,&buff[4],4);
							memcpy(&t,&buff[8],4);

							CTime tT(t);
							OutputDebug("DTU = %d status = %d time = %s(%d)",ID,status,tT.Format("%Y-%m-%d %H:%M:%S"),t);
							callGetBack2(ID,status,t);
						}

					}
				}
			}
			else if(nType == 0x69)//comm data//�����������ݰ����ֽڷ����˸ı䣬��0X68��Ϊ0X69������λ�����մ���
			{
				nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,900);//����
				if(nLen == 4)
				{
					int len = 0;
					memcpy(&len,&buff[0],4);
					if(len == 44)
					{
						int nDTU_ID;
						int nType;
						long t;
						nLen = tcpreceive(g_GetData.sSocket,(char* )buff,44,900);
						if(nLen == 44)
						{
							memcpy(&nDTU_ID,&buff[0],4);
							memcpy(&nType,&buff[4],4);
							memcpy(&t,&buff[8],4);

							CTime tT(t);
							OutputDebug("DTU = %d ID = %d time = %s(%d)",nDTU_ID,buff[13],tT.Format("%Y-%m-%d %H:%M:%S"),t);

							callGetBack(nDTU_ID,buff[13],nType,t,(BYTE* )&buff[12],32);
						}
						
					}
				}
			}
			else if(nType == 9)//Уʱ
			{
				nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,100);
				
				if(nLen == 4)
				{
					int nL = 0;
					memcpy(&nL,&buff[0],4);

					if(nL == nLen)
					{
						nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,100);

						if(nLen == 4)
						{
							long t = 0;
							memcpy(&t,&buff[0],4);

							CTime tTime(t);
							long tC = CTime::GetCurrentTime().GetTime();

							if(abs(t - tC) > 30)
							{
								//����Local
								SYSTEMTIME local;
								GetLocalTime(&local);
								local.wYear = tTime.GetYear();
								local.wMonth = tTime.GetMonth();
								local.wDay = tTime.GetDay();
								local.wHour = tTime.GetHour();
								local.wMinute = tTime.GetMinute();
								local.wSecond = tTime.GetSecond();
								
								SetLocalTime(&local);
								
								OutputDebug("���ñ���ʱ�� %d-%02d-%02d %02d:%02d:%02d",
									local.wYear,local.wMonth,local.wDay,
									local.wHour,local.wMinute,local.wSecond);
							}
							else
							{
								OutputDebug("���ñ���ʱ�� ����");
							}

						}
					}
				}

			}
			else if(nType == 101)//excute db
			{
				nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,100);

				if(nLen == 4)
				{
					int type = 0;
					memcpy(&type,&buff[0],4);

					if(type == 0)//�ɹ�
					{
						callExcuteBack(type,"",0);

						if(type == 0)
						{
							OutputDebug("�ɹ�ִ��SQL");
						}
					}
					else 
					{
						nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,500);
						if(nLen == 4)
						{
							int text_len = 0;
							memcpy(&text_len,&buff[0],4);

							if(text_len > 0 && text_len <= 1024)
							{
								nLen = tcpreceive(g_GetData.sSocket,(char* )buff,text_len,800);

								if(nLen == text_len)
								{
									callExcuteBack(type,buff,text_len);
									OutputDebug("ִ��SQL ����  %s",buff);
								}
							}
						}
					}
				}
				else if(nLen == -1)//�Ͽ�
				{
					break;
				}

			}
		}
		else
		{
			Sleep(100);
		}
	}

Reconnect:
	closesocket(g_GetData.sSocket);
	OutputDebug("����....");
	nRet = _ConnectServer(g_strPIP,g_nPort,g_GetData.sSocket);//reconnect

	if(nRet == 0)
	{
		OutputDebug("����..�ɹ�..");
		goto START;
	}
	else
	{
		Sleep(1000);;
		goto Reconnect;
	}
	g_GetData.callShutdownBack();
	g_GetData.bConnect = FALSE;

	SqlOPP2.CloseSql();
	::CoUninitialize();//����ʼ��COM��

	return 0;
}


int pascal NetComm(char* pServerIP,int nPort,
	CALL_BACK_GET_DATA callGetBack,
	CALL_BACK_GET_DATA_GPRS callGetBack2,
	CALL_BACK_EXCUTE callExcuteBack,
	CALL_BACK_SHUTDOWN callShutdownBack)
{
	char ip[32] = {0};
	strcpy(ip,GetIpByDomain(pServerIP));

	strcpy(g_strPIP,ip);
	g_nPort = nPort;

	if(g_GetData.bConnect)
	{
		return 0;
	}

	int nRet = _ConnectServer(pServerIP,nPort,g_GetData.sSocket);//get
	if(nRet != 0)
	{
		return nRet; 
	}

	g_GetData.callGetBack = callGetBack;
	g_GetData.callExcuteBack = callExcuteBack;
	g_GetData.callShutdownBack = callShutdownBack;
	g_GetData.callGetBack2 = callGetBack2;

	g_GetData.bConnect = TRUE;
	AfxBeginThread(______AFX_CHECK_METER,NULL);
	
	return 0;
}

int pascal NetSend(char *pData,int nLen)
{
	if(!g_GetData.bConnect)
		return -2;

	char buff[1024] = {0};
	int nType = 101;
	memcpy(&buff[0],&nType,4);
	memcpy(&buff[4],&nLen,4);
	memcpy(&buff[8],pData,nLen);
	return send(g_GetData.sSocket,buff,nLen + 8,0);
}

int pascal NetSendMyData(int DtuID,unsigned char MepID,unsigned char *pData,int nLen)
{
	if(!g_GetData.bConnect)
		return -2;

	char buff[1024] = {0};
	int nType = 102;
		memcpy(&buff[0],&nType,4);
	memcpy(&buff[4],&nLen,4);
	memcpy(&buff[8],&DtuID,4);
	memcpy(&buff[12],&MepID,1);
	memcpy(&buff[13],pData,nLen);
	return send(g_GetData.sSocket,buff,nLen + 13,0);
}

int pascal NetSendMyDataTrue(int DtuID, unsigned char nMeter_ID, int money, int delayTime)
{
	unsigned char buff[32]; unsigned char buff3[4]; unsigned char buff4[4];
	int i;

	if (!g_GetData.bConnect)
		return -2;
	//68  01 20 04   08     00 0D 02        00 00 00    00 06 00    04 46 00  
	//	00 32 00  00 00 00   00 00 00   00 00 00       46 14     16


	memcpy(buff3, &money, 4);
	memcpy(buff4, &delayTime, 4);

	memset(buff, 0, 32);

	buff[0] = 0x68;

	buff[1] = nMeter_ID; buff[2] = 0x20; buff[3] = 0x04; buff[4] = 0x08;

	buff[5] = buff3[0]; buff[6] = buff3[1]; buff[7] = buff3[2]; //֧�����,��λ��ǰ

	buff[8] = buff4[0]; buff[9] = buff4[1]; buff[10] = buff4[2]; buff[11] = buff4[3];//��ʱʱ�䵥λ0.1��
	buff[12] = 0x06; buff[13] = 0x00; buff[14] = 0x04; buff[15] = 0x46;
	buff[16] = 0x00; buff[17] = 0x00; buff[18] = 0x32; buff[19] = 0x00; buff[20] = 0x00;
	buff[21] = 0x00; buff[22] = 0x00; buff[23] = 0x00; buff[24] = 0x00; buff[25] = 0x00;
	buff[26] = 0x00; buff[27] = 0x00; buff[28] = 0x00;

	WORD dddd = Caluation_CRC16((BYTE*)buff, 29);
	BYTE bb[2] = { 0 };
	memcpy(bb, &dddd, 2);

	buff[29] = bb[0]; buff[30] = bb[1];

	buff[31] = 0x16;

	i = NetSendMyData(DtuID, nMeter_ID, buff, 32);

	return i;
}

int pascal NetSendMyDataTrue2(int DtuID, unsigned char nMeter_ID, int money, int delayTime,int CH)
{
	unsigned char buff[32]; unsigned char buff3[4]; unsigned char buff4[4];
	int i;

	if (!g_GetData.bConnect)
		return -2;
	//68  01 20 04   08     00 0D 02        00 00 00    00 06 00    04 46 00  
	//	00 32 00  00 00 00   00 00 00   00 00 00       46 14     16


	memcpy(buff3, &money, 4);
	memcpy(buff4, &delayTime, 4);

	memset(buff, 0, 32);

	buff[0] = 0x68;

	buff[1] = nMeter_ID; buff[2] = 0x20; buff[3] = 0x04; buff[4] = 0x08;

	buff[5] = buff3[0]; buff[6] = buff3[1]; buff[7] = buff3[2]; //֧�����,��λ��ǰ

	buff[8] = buff4[0]; buff[9] = buff4[1]; buff[10] = buff4[2]; buff[11] = buff4[3];//��ʱʱ�䵥λ0.1��
	buff[12] = 0x00;
	buff[13] = 0x00; buff[14] = 0x04; buff[15] = 0x46;
	buff[16] = 0x00; buff[17] = 0x00; buff[18] = 0x32; buff[19] = 0x00; buff[20] = 0x00;
	buff[21] = 0x00; buff[22] = 0x00; buff[23] = 0x00; buff[24] = 0x00;
	buff[25] = CH;
	buff[26] = 0x00; buff[27] = 0x00; buff[28] = 0x00;

	WORD dddd = Caluation_CRC16((BYTE*)buff, 29);
	BYTE bb[2] = { 0 };
	memcpy(bb, &dddd, 2);


	buff[29] = bb[0]; buff[30] = bb[1];

	buff[31] = 0x16;

	i = NetSendMyData(DtuID, nMeter_ID, buff, 32);

	return i;
}

int pascal NetSendMyDataTrue3(int DtuID, unsigned char nMeter_ID, int money, int delayTime, int CH, int paymoney)
{
	unsigned char buff[32]; unsigned char buff3[4]; unsigned char buff4[4];
	int i;

	if (!g_GetData.bConnect)
		return -2;
	//68  01 20 04   08     00 0D 02        00 00 00    00 06 00    04 46 00  
	//	00 32 00  00 00 00   00 00 00   00 00 00       46 14     16

	memcpy(buff3, &money, 4);
	memcpy(buff4, &delayTime, 4);

	memset(buff, 0, 32);

	buff[0] = 0x68;

	buff[1] = nMeter_ID; buff[2] = 0x20; buff[3] = 0x04; buff[4] = 0x08;

	buff[5] = buff3[0]; buff[6] = buff3[1]; buff[7] = buff3[2]; //֧�����,��λ��ǰ

	buff[8] = buff4[0]; buff[9] = buff4[1]; buff[10] = buff4[2]; buff[11] = buff4[3];//��ʱʱ�䵥λ0.1��

	
	buff[12] = 0x00;
	buff[13] = 0x00; buff[14] = 0x04; buff[15] = 0x46;
	buff[16] = 0x00; buff[17] = 0x00; buff[18] = 0x32; buff[19] = 0x00; buff[20] = 0x00; buff[21] = 0x00;
	memcpy(buff3, &paymoney, 4);
	buff[22] = buff3[0]; buff[23] = buff3[1]; buff[24] = buff3[2]; //֧�����,��λ��ǰ
	buff[25] = CH;
	buff[26] = 0x00; buff[27] = 0x00; buff[28] = 0x00;

	WORD dddd = Caluation_CRC16((BYTE*)buff, 29);
	BYTE bb[2] = { 0 };
	memcpy(bb, &dddd, 2);


	buff[29] = bb[0]; buff[30] = bb[1];

	buff[31] = 0x16;

	i = NetSendMyData(DtuID, nMeter_ID, buff, 32);

	return i;
}

int pascal NetSendMyAccount(int DtuID, unsigned char nMeter_ID, int OKorNO) //�������豸���ͽ���ȷ������
{
	unsigned char buff[32]; unsigned char buff3[4]; unsigned char buff4[4];
	int i;

	if (!g_GetData.bConnect)
		return -2;
	/*
	��λ���õ�������Ϣ��������OKȷ��֡���豸��ȷ��֡һ�����£�
	����������23ʱ����������������λ������ȷ��֡��

	68 01 20 23   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     00 00 00   46 14    16
	���01             ����           ȷ��ʶ��        ����             ����         ����             ����         ����        ����      CRCЧ��
	0X000001=1
	1������ɹ�
	*/

	memcpy(buff3, &OKorNO, 4);
	//memcpy(buff4, &delayTime, 4);

	memset(buff, 0, 32);

	buff[0] = 0x68;

	buff[1] = nMeter_ID; buff[2] = 0x20; buff[3] = 0x23; buff[4] = 0x08;

//	buff[5] = buff3[0]; buff[6] = buff3[1]; buff[7] = buff3[2]; //,��λ��ǰ

	//buff[8] = buff4[0]; buff[9] = buff4[1]; 
	buff[10] = buff3[0];//����Э������1����ɹ�
	//buff[11] = buff4[3];//
	buff[12] = 0x06; buff[13] = 0x00; buff[14] = 0x04; buff[15] = 0x46;
	buff[16] = 0x00; buff[17] = 0x00; buff[18] = 0x32; buff[19] = 0x00; buff[20] = 0x00;
	buff[21] = 0x00; buff[22] = 0x00; buff[23] = 0x00; buff[24] = 0x00; buff[25] = 0x00;
	buff[26] = 0x00; buff[27] = 0x00; buff[28] = 0x00;

	WORD dddd = Caluation_CRC16((BYTE*)buff, 29);
	BYTE bb[2] = { 0 };
	memcpy(bb, &dddd, 2);


	buff[29] = bb[0]; buff[30] = bb[1];

	buff[31] = 0x16;

	i = NetSendMyData(DtuID, nMeter_ID, buff, 32);

	return i;
}

int pascal MENGsendDown_Data(int DtuID, unsigned char nMeter_ID, unsigned int* Down_Data, unsigned char ttm)
{
	unsigned char buff[32]; unsigned char buff3[4]; unsigned char buff4[4];
	int i;

	if (!g_GetData.bConnect)
		return -2;
	/*
	��λ���õ�������Ϣ��������OKȷ��֡���豸��ȷ��֡һ�����£�
	����������23ʱ����������������λ������ȷ��֡��

	68 01 20 23   08     00 0D 00       00 00  01      00 00  60        04 00  46    00 00  32      00 00 00     00 00 00     00 00 00   46 14    16
	���01             ����           ȷ��ʶ��        ����             ����         ����             ����         ����        ����      CRCЧ��
	0X000001=1
	1������ɹ�
	*/

	memset(buff, 0, 32);
	if (ttm == 0)
	{
		buff[0] = 0x68;

		buff[1] = nMeter_ID; buff[2] = 0x20; buff[3] =14; buff[4] = 0x08;

		buff[5] = 0; buff[6] = Down_Data[0]>>8; buff[7] = Down_Data[0]&0xFF; //,��λ��ǰ
		buff[8] = 0; buff[9] = Down_Data[1] >> 8; buff[10] = Down_Data[1] & 0xFF; //,��λ��ǰ
		buff[11] = 0; buff[12] = Down_Data[2] >> 8; buff[13] = Down_Data[2] & 0xFF;  //,��λ��ǰ
		buff[14] = 0; buff[15] = Down_Data[3] >> 8; buff[16] = Down_Data[3] & 0xFF;  //,��λ��ǰ
		buff[17] = 0; buff[18] = Down_Data[4] >> 8; buff[19] = Down_Data[4] & 0xFF;  //,��λ��ǰ
		buff[20] = 0; buff[21] = Down_Data[5] >> 8; buff[22] = Down_Data[5] & 0xFF;  //,��λ��ǰ
		buff[23] = 0; buff[24] = Down_Data[6] >> 8; buff[25] = Down_Data[6] & 0xFF;  //,��λ��ǰ
		buff[26] = 0; buff[27] = Down_Data[7] >> 8; buff[28] = Down_Data[7] & 0xFF; //,��λ��ǰ

		WORD dddd = Caluation_CRC16((BYTE*)buff, 29);
		BYTE bb[2] = { 0 };
		memcpy(bb, &dddd, 2);


		buff[29] = bb[0]; buff[30] = bb[1];

		buff[31] = 0x16;
	}
	else if (ttm == 8)
	{
		buff[0] = 0x68;

		buff[1] = nMeter_ID; buff[2] = 0x20; buff[3] = 16; buff[4] = 0x08;

		buff[5] = 0; buff[6] = Down_Data[8] >> 8; buff[7] = Down_Data[8] & 0xFF;  //,��λ��ǰ
		buff[8] = 0; buff[9] = Down_Data[9] >> 8; buff[10] = Down_Data[9] & 0xFF; //,��λ��ǰ
		buff[11] = 0; buff[12] = Down_Data[10] >> 8; buff[13] = Down_Data[10] & 0xFF; ; //,��λ��ǰ
		buff[14] = 0; buff[15] = Down_Data[11] >> 8; buff[16] = Down_Data[11] & 0xFF;  //,��λ��ǰ
		buff[17] = 0; buff[18] = Down_Data[12] >> 8; buff[19] = Down_Data[12] & 0xFF;  //,��λ��ǰ
		buff[20] = 0; buff[21] = Down_Data[13] >> 8; buff[22] = Down_Data[13] & 0xFF;  //,��λ��ǰ
		buff[23] = 0; buff[24] = Down_Data[14] >> 8; buff[25] = Down_Data[14] & 0xFF;  //,��λ��ǰ
		buff[26] = 0; buff[27] = Down_Data[15] >> 8; buff[28] = Down_Data[15] & 0xFF;  //,��λ��ǰ

		WORD dddd = Caluation_CRC16((BYTE*)buff, 29);
		BYTE bb[2] = { 0 };
		memcpy(bb, &dddd, 2);


		buff[29] = bb[0]; buff[30] = bb[1];

		buff[31] = 0x16;
	}
	i = NetSendMyData(DtuID, nMeter_ID, buff, 32);

	return i;
}

int pascal GetQueryData(char *pData,int nLen,char* pRetData,int& nRetLen)
{
	return -2;
}

int pascal RegSystem()
{

	return 0;
}

int pascal myfction(int data1)
{
  return data1+2;
}
