// NetComm.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "NetComm.h"

#include "unzip.h"
#include "Des.h"

#include "DialogReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_nThisVer = 4;//版本号

CString GetCurrentPathName()
{
	char buf[MAX_PATH] = {0};
	DWORD len = MAX_PATH;
	
	GetModuleFileName(NULL,buf,len);
	CString exe;
	exe.Format("%s",buf);
	int n = exe.ReverseFind('\\');
	exe = exe.Left(n);
	
	return exe;
}


CString GetSystemError(int code)
{
	CString strR;
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM|
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		code,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);
	strR.Format("%s",lpMsgBuf);
	
	return strR;
	
}

CString GetTempFileNames()
{
	CString strName;
	char szTempName[MAX_PATH]; 
	
	TCHAR   szPathCopy[_MAX_PATH]; 
	GetTempPath(_MAX_PATH,szPathCopy); 
	GetTempFileName(szPathCopy, // dir. for temp. files 
		"NEW",                // temp. file name prefix 
		0,                    // create unique name 
		szTempName);          // buffer for name 
	
	strName.Format("%s",szTempName);
	return strName;
}

BOOL FileIsExist(CString name)
{
	CStdioFile file;
	if(file.Open(name,CFile::modeRead))
	{
		file.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL UnZipFile(CString strFile)
{
	char strFileName[MAX_PATH] = {0};
	strcpy(strFileName,strFile);
	
	char path[MAX_PATH] = {0};
	
	int n = strFile.ReverseFind('\\');
	sprintf(path,"%s",strFile.Left(n));
	//	SetCurrentDirectory(path);
	
	char strDestName[MAX_PATH] = {0};
	HZIP hz = OpenZip(strFileName,0);
	ZIPENTRY ze; 
	GetZipItem(hz,-1,&ze); 
	int numitems=ze.index;
	for (int i=0; i<numitems; i++)
	{
		GetZipItem(hz,i,&ze);
		sprintf(strDestName,"%s\\%s",path,ze.name);
		
		UnzipItem(hz,i,strDestName);
	}
	
	CloseZip(hz);
	
	return TRUE;
}


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
/*	HRSRC hRes; 
	HINSTANCE hInst = AfxGetResourceHandle(); 
	BOOL bResult = FALSE; 
	
	DWORD dwLen=0; 
	
	hRes = FindResource(hInst,MAKEINTRESOURCE(IDR_HTML_MAP),"HTML"); 
	if(hRes != NULL) 
	{
		dwLen = SizeofResource(hInst,hRes); 
		if(dwLen > 0) 
		{
			HGLOBAL hGlob = LoadResource(hInst,hRes); 
			if(hGlob != NULL) 
			{
				LPVOID lpData = LockResource(hGlob); 
				
				if(lpData != NULL) 
				{
					CFile   f; 
					if(f.Open(strFileName,CFile::modeCreate|CFile::modeWrite)) 
					{
						f.WriteHuge(lpData,dwLen); 
						f.Close(); 
						bResult=TRUE; 
					}
					else
					{
						AfxMessageBox("写入资源错误.");
					}
				} 
				else
				{
					AfxMessageBox("读取资源数据错误.");
				}
				
				FreeResource(hGlob); 
			} 
			else
			{
				AfxMessageBox("获取资源错误.");
			}
		} 
		else
		{
			AfxMessageBox("资源大小错误.");
		}
	} 
	else
	{
		AfxMessageBox("未找到资源.");
		int d = GetLastError();		
	}
	
	return bResult;
	*/
	return TRUE;
}

char g_strPIP[32] = {0};
int g_nPort = 0;

int _ConnectServer(char* ip,int port,SOCKET &s)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1,1);
	WSAStartup(wVersionRequested, &wsaData);
	
	char strError[1024] = {0};
	sockaddr_in srv;
	memset(&srv,0,sizeof(srv));
	srv.sin_addr.S_un.S_addr = inet_addr(ip);
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s <= 0)
	{
		int error = GetLastError();
		sprintf(strError,"socket error %d (%s) File:%s Line = %d ",error,GetSystemError(error),__FILE__,__LINE__);
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
		sprintf(strError,"bind error  %d (%s) File:%s Line = %d ",error,GetSystemError(error),__FILE__,__LINE__);
		OutputDebug(strError);
		return 2;
	}
	if(connect(s ,(struct sockaddr*)&srv, sizeof(srv)) != 0)
	{
		int error = GetLastError();
		closesocket(s);
		
		sprintf(strError,"Connect error %d  File:%s Line = %d ",error,__FILE__,__LINE__);
		OutputDebug(strError);
		
		return 3;
	}
// 	CString strPath = GetCurrentPathName();
// 	CString strFileName = 
// 	GetModelFromRes
// 	DeleteFile();
	return 0;
}


CTime ____GetBuildDateTime()
{
	char s_month[5];
	int month, day, year;
	char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
	sscanf(__DATE__, "%s %d %d", s_month, &day, &year);
	month = (strstr(month_names, s_month)-month_names)/3 + 1;
	
	int h,m,s;
	sscanf(__TIME__, "%d:%d:%d",&h,&m,&s);
	
	CTime t(year,month,day,h,m,s);
	
	return t;
}

/////////////////////////////////////////////////////////////////////////////
// CNetCommApp

BEGIN_MESSAGE_MAP(CNetCommApp, CWinApp)
	//{{AFX_MSG_MAP(CNetCommApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetCommApp construction

CNetCommApp::CNetCommApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNetCommApp object

CNetCommApp theApp;


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

char* GetIpByDomain(char* pHost)//域名 转ip地址
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1,1);
	WSAStartup(wVersionRequested, &wsaData);
	
    static char ipv4[16] = {0};
    if (inet_addr(pHost) != INADDR_NONE)
	{
		strcpy(ipv4,pHost);
    }
	else
	{
		hostent* answer = gethostbyname(pHost);
		if (answer != NULL) 
		{
			BYTE ip[4] = {0};
			memcpy(ip,answer->h_addr_list[0],4);
			
			sprintf(ipv4,"%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
		}		
	}
	return ipv4;
}

void CreateMapFile(char* name,char* pKey,int nKeyLen)
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
	if(!file.Open(name,CFile::modeWrite|CFile::modeCreate))
	{
		return;
	}
	strcat(str[9],pKey);
	
	for(int i = 0;i < 21;i ++)
	{
		file.Write(str[i],strlen(str[i]));
	}
	file.Close();
	
}


UINT ______AFX_DOWN_LOAD(LPVOID pParam)
{
	_DOWNLOAD_DATA_* pData = (_DOWNLOAD_DATA_* )pParam;

	char strMapFile[MAX_PATH] = {0};
	sprintf(strMapFile,"%s\\js\\map.html",GetCurrentPathName());
	DeleteFile(strMapFile);
	// 	CString strPath = GetCurrentPathName();
	// 	CString strFileName = 

	char name[MAX_PATH] = {0};
	strcpy(name,pData->name);

	CALL_BACK_DOWNLOAD callBack = pData->callBack;

	char strError[1024] = {0};

	sprintf(strError,"连接成功，版本验证 %d",g_nThisVer);
	callBack(5,strError,strlen(strError));

	int nType = 100;
	char buff[1024 * 100] = {0};
	memcpy(&buff[0],&nType,4);
	memcpy(&buff[4],&g_nThisVer,4);
	
	int n = send(pData->sSocket,buff,8,0);//版本
	if(n < 0)
	{
		int d = GetLastError();
		closesocket(pData->sSocket);

		sprintf(strError,"send error File:%s Line = %d ",__FILE__,__LINE__);
		callBack(1,strError,strlen(strError));
		delete pData;
		return 0;
	}

	OutputDebug("发送本地版本 %d",g_nThisVer);
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

		sprintf(strError,"版本验证超时 File:%s Line = %d ",__FILE__,__LINE__);
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

			OutputDebug("文件大小 = %d\n\n\n",nFileLen);
			sprintf(strError,"开始下载文件,文件大小 %d",nFileLen);
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

			sprintf(strError,"文件下载进度 %.2f %%",nDownLoadSize * 100.0f / nFileLen);
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

				strcpy(strError,"数据库下载完成. ");
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
	
	//	strNew.Format("%s",des.GetPlaintextAnyLength());
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
			//	AfxMessageBox("注册码错误.");
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
				//m_edDay.SetWindowText("无限制版本");
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
					AfxMessageBox("授权已经到期.");

					return 1;//授权已经到期
				}
				//m_edDay.SetWindowText(strY + "-" + strM + "-" + strD);
			}
			
		}
	}
//	AfxMessageBox("注册码错误.");
	return -1;//无注册码
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

	//	if(!dlg.m_bOK)
			exit(0);
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

	//	if(!dlg.m_bOK)
			exit(0);
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


_GET_DATA_ g_GetData;
UINT ______AFX_CHECK_METER(LPVOID pParam)
{
	int nRet = 0;

	CALL_BACK_GET_DATA callGetBack = g_GetData.callGetBack;
	CALL_BACK_EXCUTE callExcuteBack = g_GetData.callExcuteBack;
	CALL_BACK_GET_DATA_GPRS callGetBack2 = g_GetData.callGetBack2;

START:
	char strError[1024] = {0};

	g_GetData.bConnect = TRUE;
	char strS[32] = {0};
	strS[0] = 0x9;

	DWORD dwLastRecv = 0;
	dwLastRecv = 0;//GetTickCount();
	while(TRUE)
	{
		if(GetTickCount() - dwLastRecv > 30 * 1000) //定期查询服务器上的时间，更新自己的时间
		{
			dwLastRecv = GetTickCount();

			if(send(g_GetData.sSocket,strS,1,0) <= 0)
			{
				break;
			}
			OutputDebug("查询.....");

		}
		char buff[1024] = {0};
		int nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,100);//类型

		if(nLen > 0)
		{
			OutputDebug("%d  %s",nLen,buff);
		}
		if(nLen == -1)//断开
		{
			break;
		}
		else if(nLen == 4)
		{
			int nType = 0;
			memcpy(&nType,&buff[0],4);

			if(nType == 0x67)//GetData dtu  //函数里面数据包首字节发生了改变，从0X68改为0X67，让上位机接收处理
			{
				nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,900);//长度
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
			else if(nType == 0x69)//comm data//函数里面数据包首字节发生了改变，从0X68改为0X69，让上位机接收处理
			{
				nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,900);//长度
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
/*			else if(nType == 0x68)//GetData meter
			{
				nLen = tcpreceive(g_GetData.sSocket,(char* )buff,4,900);//长度
				if(nLen == 4)
				{
					int len = 0;
					memcpy(&len,&buff[0],4);

					if(len == sizeof(_DATA_METER))
					{
						nLen = tcpreceive(g_GetData.sSocket,(char* )buff,sizeof(_DATA_METER),900);
						if(nLen == sizeof(_DATA_METER))
						{
							_DATA_METER data = {0};
							
							memcpy(&data,&buff[0],nLen);

// 							OutputDebug("编号 = %d 已用电量 = %d 剩余电量 = %d 已用钱 %d  剩余钱 %d 单价 %d 流量 %d 瞬间流量 %d",
// 								data.nID,data.nUedE,data.nLeftE,data.nUsedM,data.nLeftM,data.nRate,data.nFlow,data.nFlow2);
							CTime t(data.lTime);
							OutputDebug("DTU = %d ID = %d Time = %s(long = %d)",data.nDTU_ID,data.nID,t.Format("%Y-%m-%d %H:%M:%S"),data.lTime);
							callGetBack(&data);


							dwLastRecv = GetTickCount();
						}
					}
				}
				else if(nLen == -1)//断开
				{
					break;
				}
			}*/
			else if(nType == 9)//校时
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
								//设置Local
								SYSTEMTIME local;
								GetLocalTime(&local);
								local.wYear = tTime.GetYear();
								local.wMonth = tTime.GetMonth();
								local.wDay = tTime.GetDay();
								local.wHour = tTime.GetHour();
								local.wMinute = tTime.GetMinute();
								local.wSecond = tTime.GetSecond();
								
								SetLocalTime(&local);
								
								OutputDebug("设置本地时间 %d-%02d-%02d %02d:%02d:%02d",
									local.wYear,local.wMonth,local.wDay,
									local.wHour,local.wMinute,local.wSecond);
							}
							else
							{
								OutputDebug("设置本地时间 忽略");
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

					if(type == 0)//成功
					{
						callExcuteBack(type,"",0);

						if(type == 0)
						{
							OutputDebug("成功执行SQL");
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
									OutputDebug("执行SQL 错误  %s",buff);
								}
							}
						}
					}
				}
				else if(nLen == -1)//断开
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
	OutputDebug("重连....");
	nRet = _ConnectServer(g_strPIP,g_nPort,g_GetData.sSocket);//reconnect

	if(nRet == 0)
	{
		OutputDebug("重连..成功..");
		goto START;
	}
	else
	{
		Sleep(1000);;
		goto Reconnect;
	}
	g_GetData.callShutdownBack();
	g_GetData.bConnect = FALSE;

	return 0;

}


int pascal NetComm(char* pServerIP,int nPort,CALL_BACK_GET_DATA callGetBack,CALL_BACK_GET_DATA_GPRS callGetBack2,CALL_BACK_EXCUTE callExcuteBack,CALL_BACK_SHUTDOWN callShutdownBack)
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
//	g_GetData.callGetBack2 = callGetBack2;
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
WORD Caluation_CRC16(BYTE Buff[], int nSize)
{
	WORD nOld;
	WORD nRet = 0xffff;
	unsigned short i,j;
	for(i=0; i<nSize; i++)
	{
		nRet ^= Buff[i];
		for(j=0; j<8; j++)
		{
			nOld = nRet;
			nRet >>= 1;
			if(nOld&0x0001)
				nRet ^= 0xa001;
		}
	}
	return nRet;
}
int pascal NetSendMyDataTrue(int DtuID,unsigned char nMeter_ID, int money,int delayTime  )
{


	unsigned char buff[32] ;unsigned char buff3[4] ;unsigned char buff4[4] ;
	int i;

	if(!g_GetData.bConnect)
		return -2;
   //68  01 20 04   08     00 0D 02        00 00 00    00 06 00    04 46 00  
//	00 32 00  00 00 00   00 00 00   00 00 00       46 14     16


	memcpy(buff3,&money,4);
	memcpy(buff4,&delayTime,4);

    memset(buff,0,32);

    buff[0]=0x68;

    buff[1]=nMeter_ID;buff[2]=0x20;buff[3]=0x04;buff[4]=0x08;

	buff[5]=buff3[0];buff[6]=buff3[1]; buff[7]=buff3[2]; //支付金额,低位在前

	buff[8]=buff4[0];buff[9]=buff4[1];buff[10]=buff4[2];buff[11]=buff4[3];//延时时间单位0.1秒
    buff[12]=0x06;buff[13]=0x00;buff[14]=0x04;buff[15]=0x46;
    buff[16]=0x00;buff[17]=0x00;buff[18]=0x32;buff[19]=0x00;buff[20]=0x00;
    buff[21]=0x00;buff[22]=0x00;buff[23]=0x00;buff[24]=0x00;buff[25]=0x00;
	buff[26]=0x00;buff[27]=0x00;buff[28]=0x00;
	
	WORD dddd = Caluation_CRC16((BYTE* )buff,29);
	BYTE bb[2] = {0};
	memcpy(bb,&dddd,2);
	
	
	buff[29]=bb[0];buff[30]=bb[1];

	buff[31]=0x16;

	i=NetSendMyData(DtuID,nMeter_ID,buff,32);

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