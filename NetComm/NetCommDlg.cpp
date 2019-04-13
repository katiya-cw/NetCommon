
// NetCommDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "NetComm.h"
#include "NetCommDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "httpserver.h"
#include "LogicCenter.h"
#include "DeviceEvent.h"
#include "MD5/common.h"  
#include "db/DbOperate.h""
#include "../MyDefine.h"
#include "ThreadPool.h"
#include "LogUtil/easylogging++.h"
//#include "Http/HttpClientManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "db/DbOperate.h""

#ifdef updataOnOffDTU
#include<thread>

int bmm;

void updataSQL(int mm)
{

	DWORD dwRecvTime;//����ʱ�� 
	DWORD dwRecvTime2;
	DWORD dwRecvTime3;
	DWORD CJTime;
	CTime t;
	t = CTime::GetCurrentTime();
	dwRecvTime = t.GetTime();
	CString csStartTime = t.Format("%Y%m%d%H%M%S"); //���ɱ�׼ʱ���ʽ

													//CString s("2001-8-29 19:06:23");
	int nYear, nMonth, nDate, nHour, nMin, nSec;
	//sscanf(s, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
	//CTime myT(nYear, nMonth, nDate, nHour, nMin, nSec);
	//dwRecvTime2 = myT.GetTime();

	//CString s1("2019-02-25 13:16:00.000");

	//sscanf(s1, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
	//CTime myT1(nYear, nMonth, nDate, nHour, nMin, nSec);
	//dwRecvTime3 = myT1.GetTime();
	//if (dwRecvTime >= dwRecvTime3)
	//{
	//	dwRecvTime2 = dwRecvTime - dwRecvTime3; //ʱ����������
	//}
	::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
	CDbOperate SqlOPP;
	//	AfxMessageBox("0�ɹ�");
	SqlOPP.OpenSql();
	//	AfxMessageBox("1�ɹ�"); //ConnectDate
	CString old_dwRecvTime;
	// old_dwRecvTime= SqlOPP.MyQuerySQLconnectTime(7003);
	SqlOPP.MyQuerySQLconnectTime();
	map <DWORD, CString>::iterator it;
	for (it = SqlOPP.devTimeMap.begin(); it != SqlOPP.devTimeMap.end(); ++it)
	{

		if (it->first == 595)
		{
			t = CTime::GetCurrentTime();
		}
		//	cout << "key: " << it->first << " value: " << it->second << endl;
		old_dwRecvTime = it->second;
		sscanf(old_dwRecvTime, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
		CTime myT2(nYear, nMonth, nDate, nHour, nMin, nSec);

		dwRecvTime3 = myT2.GetTime();
		if (dwRecvTime >= dwRecvTime3)
		 {
		CJTime = dwRecvTime - dwRecvTime3;
	     }
		if (CJTime > 600)//10���� ,Ӧ�ô���8���ӱȽϰ�ȫ  �������ߵ�ʱ���豸����Ϊ���ߣ�����07���������ر�ע��
		{   
			long pkkk = it->first;
			//if (pkkk == 595)
			//{
			//	pkkk = 595;
			//}
			SqlOPP.UpdateSQLconnectTime(pkkk, 0);  //�����豸������״̬	//����
		}

	}

	SqlOPP.CloseSql();
	::CoUninitialize();//����ʼ��COM��


}
#endif

extern CString URLEncode(CString sIn);

ThreadPool threadPool(10);

int connectFlgNum=0;
int SQLwriteFlag = 0;
int myDTU_ID =0;
int myID= 0;

int SQLwriteFlag2 = 0;
int myDTU_ID2 = 0;
int myID2 = 0;

int SQLwriteFlag3 = 0;
int SQLwriteFlag4 = 0;
int myDTU_ID3 = 0;
#ifdef SENDforOK
unsigned char DevSendTimes[SENDTIMES_LENS];
#endif
HANDLE g_hOut = 0;
int g_nVer = 1;//�汾
//�������߳����б�־
static bool s_ServerRun = false;
static unsigned int   hth1;
//�߳�
unsigned __stdcall ServerWork(void *pInfo)
{

#ifdef updataOnOffDTU
	//CDataBase* view;
	//std::thread updateThread3([](CDataBase* view) {
	//	while (true) {
	//		view->LoadBaseTables2();
	//		Sleep(5000);
	//	}
	//}, view);


	std::thread mengThread2([](int bmm) {
		while (true) {
			updataSQL(bmm);
			//Sleep(180000); //180��=3����
			Sleep(30000);
		}
	}, 3);
	//while (1)
	//{
	// Sleep(60000);
	//}


#endif

	//TREAD_INFO *p = (TREAD_INFO*)pInfo;
	CHttpServer *server = (CHttpServer*)pInfo;

	//server.Init(p->port,p->ip);
	server->SetRecvCallBack(callback_OnServerRecv);
	while (s_ServerRun)
	{
		server->work();
	}
	server->Release();
	return 0;
}
unsigned __stdcall ClientWork(void* data) {
	OutputDebug("begin to send request to server.");
	CString text((char*)data);
	delete[] data;

	//CString ret = CHttpClientManager::getInstance()->request(text);

	return 0;
}
// CNetCommDlg �Ի���



CNetCommDlg::CNetCommDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NETCOMM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNetCommDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLEAR, &CNetCommDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_START, &CNetCommDlg::OnBnClickedStart)
	ON_MESSAGE(SERVER_INFO_STRING_MSG, &CNetCommDlg::OnServerInfoMsg)
	ON_MESSAGE(CLIENT_INFO_STRING_MSG, &CNetCommDlg::OnClientInfoMsg)
	ON_MESSAGE(SERVER_URL_STRING_MSG, &CNetCommDlg::OnServerURLMsg)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CNetCommDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CNetCommDlg::OnBnClickedButtonTest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_START2, &CNetCommDlg::OnBnClickedStart2)
	ON_BN_CLICKED(IDC_START3, &CNetCommDlg::OnBnClickedStart3)
END_MESSAGE_MAP()



// CNetCommDlg ��Ϣ�������

BOOL CNetCommDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	el::Loggers::configureFromGlobal("log.conf");
	CString startLog = "The program has started.";
	LOG(INFO) << startLog;

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CString bmm;
	bmm.Format("%d", httpRevUsePort);
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->SetWindowText(bmm);
	//((CEdit*)GetDlgItem(IDC_EDIT_URL))->SetWindowText("http://127.0.0.1:8090?id=201611011516234&action=50000&time=201611011516234&xx=111111&xx2=222222&xx3=333333&xx4=444444&xx5=555555");
	CString hgg;
	hgg.Format("http://127.0.0.1:%d?machineid=56&paymoney=6.0&id=115347&md5val=187c8207b32ee9eb37ccff8a9bf15b02&time=201611130605&blance=-1", httpRevUsePort);
	((CEdit*)GetDlgItem(IDC_EDIT_URL))->SetWindowText(hgg);

	//machineid = 56 & paymoney = 6.00&id = 115347 & md5val = kfwjeklx15151sdw23&time = 201611130605 & blance = -1
	/*
	((CEdit*)GetDlgItem(IDC_EDIT_C_P1))->SetWindowText("xx1=3211111");
	((CEdit*)GetDlgItem(IDC_EDIT_C_P2))->SetWindowText("xx2=3211111");
	((CEdit*)GetDlgItem(IDC_EDIT_C_P3))->SetWindowText("xx3=3211111");
	((CEdit*)GetDlgItem(IDC_EDIT_C_P4))->SetWindowText("xx4=3211111");
	((CEdit*)GetDlgItem(IDC_EDIT_C_P5))->SetWindowText("xx5=3211111");
	((CEdit*)GetDlgItem(IDC_EDIT_C_P6))->SetWindowText("xx6=3211111");
	((CEdit*)GetDlgItem(IDC_EDIT_C_P7))->SetWindowText("xx7=3211111");
	((CEdit*)GetDlgItem(IDC_EDIT_C_P8))->SetWindowText("xx8=3211111");
	*/
	
	CRichEditCtrl *pEServer = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_SERVER));
	pEServer->SetWindowText("----------------�����---------------");
	CRichEditCtrl *pEClient = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_CLIENT));
	pEClient->SetWindowText("----------------�ͻ���---------------");

	CString strIP = "127.0.0.1";
	DWORD dwIP = ntohl(inet_addr(strIP));
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS))->SetAddress(dwIP);
	CDeviceEvent::InitLock();




//-----------------------����HTTP����---------START-------------

	DWORD   dwExitCode;

	//SetTimer(101, 200, NULL); //3��1��
							  //��ʱ����
							  /*
							  if (nIDEvent == 101)
							  {		connectFlgNum++;
							  if (connectFlgNum > 400) //��20����
							  {			connectFlgNum = 0;
							  }
							  }
							  */

							  /**/
	::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
	CDbOperate SqlOPP;
	//	AfxMessageBox("0�ɹ�");
	SqlOPP.OpenSql();
	//	AfxMessageBox("1�ɹ�");

	//SqlOPP.MyQuerySQL(23);
#ifndef  updataOnOffDTU
	//��ʼ����ʱ�豸�����ߣ�
	SqlOPP.myUpdateSQL(0);//��ʼ�������豸�����ߣ�
						  //	AfxMessageBox("2�ɹ�");
#endif
	SqlOPP.CloseSql();
	::CoUninitialize();//����ʼ��COM��




	//	AfxMessageBox("3�ɹ�");
	/*
	FILE* f = fopen("MYLOG.TXT", "a+");
	char buff[30];
	memset(buff, 0x33, 30);
	fwrite(buff, 30, 1, f);
	//	fread(buff, 30, 1, f);
	fclose(f);
	*/
	g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//OutputDebug2("�汾 %d", g_nVer);
	//	OutputDebug("����汾ʱ�� %s", GetBuildDateTime().Format("%Y-%m-%d %H:%M:%S"));

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (s_ServerRun == false)
	{
		s_ServerRun = true;
		DWORD dwIP;
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS))->GetAddress(dwIP);
		m_threadInfo.ip = htonl(dwIP);
		m_threadInfo.port = httpRevUsePort;
		m_server.Init(m_threadInfo.port, m_threadInfo.ip);
		hth1 = _beginthreadex(NULL, 0, ServerWork, (void*)&m_server, 0, 0);

		//	GetExitCodeThread((HANDLE)hth1, &dwExitCode);
		//_beginthreadex(NULL, 0, ServerWork, (void*)&m_threadInfo, 0, 0);

		((CButton*)GetDlgItem(IDC_START))->SetWindowText("ֹͣ");
	}
	else
	{
		s_ServerRun = false;
		m_server.Release();
		//_endthreadex(hth1);
		//TerminateThread(temp2);
		//	CloseHandle(hth1);

		Sleep(50);
		((CButton*)GetDlgItem(IDC_START))->SetWindowText("����");
	}

//---------------------����HTTP����------END-------------
	//----------------�����ڶ������ӷ������Ĵ���------------
	if (!::IsWindow(m_dlg.m_hWnd))
	{
		m_dlg.Create(IDD_TEST_DIALOG, NULL);
	}
	m_dlg.ShowWindow(SW_SHOW);
//----------------------

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNetCommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNetCommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNetCommDlg::OnBnClickedClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CRichEditCtrl *pEServer = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_SERVER));
	pEServer->SetWindowText("");
	CRichEditCtrl *pEClient = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_CLIENT));
	pEClient->SetWindowText("");
	CRichEditCtrl *pEURL = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_URL));
	pEURL->SetWindowText("");
	Invalidate();
}
struct mystruct
{
	
	char cha;
	char end;
	char end2;
	char i;
	char ii;
};
void myOUTerr(unsigned char err)
{
#ifdef myDebugTXT
	OutputDebug("myTEST  myOUTerr()  start..");

	FILE *stream;
	unsigned char temp1, temp2;
	//char buff[30];
	struct mystruct s;
	if ((stream = fopen("MYLOG.TXT", "a+")) == NULL) /* open file TEST.$$$ */
	{
		fprintf(stderr, "Cannot open output file.\n");
		//	return 1;
	}
	//	fwrite(pch, "aaaa\r\n", 5, 1, fp);
	//buff=
//	fwrite("\r\n", 2, 1, stream); /* ����*/
	s.cha = 'E';
	s.end = 'R';
	s.end2 = 'R';
	temp1 = ((err>>4) & 0x0f) + 0x30;
	temp2= (err & 0x0f) + 0x30;
	s.i =temp1;
	s.ii = temp2;
	fwrite(&s, sizeof(s), 1, stream); /* д��struct�ļ�*/
	fclose(stream); /*�ر��ļ�*/
	OutputDebug("myTEST  myOUTerr()   end..");
	//	return 0;
#endif
}
void myOUTerrFirst(unsigned char err)
{
#ifdef myDebugTXT
	FILE *stream;
	unsigned char temp1, temp2;
	//char buff[30];
	struct mystruct s;
	OutputDebug("myTEST  myOUTerrFirst()  start..");
	if ((stream = fopen("MYLOG.TXT", "a+")) == NULL) /* open file TEST.$$$ */
	{
		fprintf(stderr, "Cannot open output file.\n");
		//	return 1;
	}
	//	fwrite(pch, "aaaa\r\n", 5, 1, fp);
	//buff=
	fwrite("\r\n", 2, 1, stream); /* ����*/
	s.cha = 'E';
	s.end = 'R';
	s.end2 = 'R';
	temp1 = ((err >> 4) & 0x0f) + 0x30;
	temp2 = (err & 0x0f) + 0x30;
	s.i = temp1;
	s.ii = temp2;
	fwrite(&s, sizeof(s), 1, stream); /* д��struct�ļ�*/
	fclose(stream); /*�ر��ļ�*/
	OutputDebug("myTEST  myOUTerrFirst()   end..");				//	return 0;
#endif
}
void CNetCommDlg::OnBnClickedStart()
{  
	//myOUTerr(0x97);
	myOUTerrFirst(0x00);//��������ʱ��
	
	OutputServerString("��ʾ���յ�PHP���������������:\n");
	OutputClientString("��ʾ���ظ�PHP������������:\n");
	DWORD   dwExitCode;

//	SetTimer(101,200, NULL); //3��1��
	//��ʱ����
	/*
	if (nIDEvent == 101)
	{		connectFlgNum++;
		if (connectFlgNum > 400) //��20����
		{			connectFlgNum = 0;
		}
	}
	*/

/**/
	::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
	CDbOperate SqlOPP;
	//	AfxMessageBox("0�ɹ�");
	SqlOPP.OpenSql();
	//	AfxMessageBox("1�ɹ�");

	//SqlOPP.MyQuerySQL(23);

	//��ʼ����ʱ�豸�����ߣ�
	SqlOPP.myUpdateSQL(0);//��ʼ�������豸�����ߣ�
						  //	AfxMessageBox("2�ɹ�");
	SqlOPP.CloseSql();
	::CoUninitialize();//����ʼ��COM��
//	AfxMessageBox("3�ɹ�");
/*
	FILE* f = fopen("MYLOG.TXT", "a+");
	char buff[30];
	memset(buff, 0x33, 30);
	fwrite(buff, 30, 1, f);
//	fread(buff, 30, 1, f);
	fclose(f);
*/
		g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		
		OutputDebug2("�汾 %d", g_nVer);
	//	OutputDebug("����汾ʱ�� %s", GetBuildDateTime().Format("%Y-%m-%d %H:%M:%S"));
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (s_ServerRun == false)
	{
		s_ServerRun = true;		
		DWORD dwIP;
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS))->GetAddress(dwIP);
		m_threadInfo.ip = htonl(dwIP);
		m_threadInfo.port = httpRevUsePort;
		m_server.Init(m_threadInfo.port, m_threadInfo.ip);
		hth1 = _beginthreadex(NULL, 0, ServerWork, (void*)&m_server, 0, 0);
		
	//	GetExitCodeThread((HANDLE)hth1, &dwExitCode);
	    //_beginthreadex(NULL, 0, ServerWork, (void*)&m_threadInfo, 0, 0);

		((CButton*)GetDlgItem(IDC_START))->SetWindowText("ֹͣ");
	}
	else
	{
		s_ServerRun = false;
		m_server.Release();
		//_endthreadex(hth1);
		//TerminateThread(temp2);
	//	CloseHandle(hth1);

		Sleep(50);
		((CButton*)GetDlgItem(IDC_START))->SetWindowText("����");
	}	
}

//������Ϣ��ʾ
LRESULT CNetCommDlg::OnServerInfoMsg(WPARAM wparam, LPARAM lparam)
{
	CRichEditCtrl *pE = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_SERVER));
	pE->SetSel(0, -1);
	pE->SetWindowText(pE->GetSelText()+"\n"+ CString((char*)wparam));
	pE->LineScroll(pE->GetLineCount());
	Invalidate();
	return NULL;
}
//������Ϣ��ʾ
LRESULT CNetCommDlg::OnClientInfoMsg(WPARAM wparam, LPARAM lparam)
{
	CRichEditCtrl *pE = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_RET));
	pE->SetSel(0, -1);
	pE->SetWindowText(pE->GetSelText() + "\n" + CString((char*)wparam));
	pE->LineScroll(pE->GetLineCount());
	Invalidate();
	return NULL;
}

LRESULT CNetCommDlg::OnServerURLMsg(WPARAM wparam, LPARAM lparam)
{
	CString URL = CString((char*)wparam);
	CRichEditCtrl *pE = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_URL));
	pE->SetSel(0, -1);
	pE->SetWindowText(pE->GetSelText() + "\n" + URL);
	pE->LineScroll(pE->GetLineCount());
	ShowServerParam(URL);
	Invalidate();
	return NULL;
}

void CNetCommDlg::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString text;
	((CEdit*)GetDlgItem(IDC_EDIT_URL))->GetWindowText(text);

	text = text + GetClientParam();

	CRichEditCtrl *pE = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT_CLIENT));
	pE->SetSel(0, -1);
	pE->SetWindowText(pE->GetSelText() + "\n" + CString((char*)text.GetBuffer(0)));
	pE->LineScroll(pE->GetLineCount());
	Invalidate();
	OutputDebug("send http request.");
	char *buf = new char[text.GetLength() + 1];
	memset(buf, 0, text.GetLength() + 1);
	memcpy(buf, text.GetBuffer(), text.GetLength());
	_beginthreadex(NULL, 0, ClientWork, (void*)buf, 0, 0);

	//OutputClientString(ret);
}
void SendBACKtoPHPserver(CString text)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//CString ret = CHttpClientManager::getInstance()->request(text);
//	OutputClientString(ret);
}
//��ʾ����������
void CNetCommDlg::ShowServerParam(CString param)
{
	DWORD ID[] = { IDC_EDIT_S_P1,IDC_EDIT_S_P2,IDC_EDIT_S_P3,IDC_EDIT_S_P4,
				   IDC_EDIT_S_P5,IDC_EDIT_S_P6,IDC_EDIT_S_P7,IDC_EDIT_S_P8 };
	int i = 0;
	vector<CString> vec = GetURLParamValue(param);
	int size = vec.size();
	if (vec.empty())
	{
		return;
	}
	while (i < ARRAYSIZE(ID) && i < size)
	{
		GetDlgItem(ID[i])->SetWindowText(vec[i]);
		i++;
	}
}
//��ȡ�ͻ��������
CString CNetCommDlg::GetClientParam()
{
	DWORD ID[] = { IDC_EDIT_C_P1,IDC_EDIT_C_P2,IDC_EDIT_C_P3,IDC_EDIT_C_P4,
				   IDC_EDIT_C_P5,IDC_EDIT_C_P6,IDC_EDIT_C_P7,IDC_EDIT_C_P8 };
	CString ret("?");
	for (int i = 0; i < ARRAYSIZE(ID);i++)
	{
		CString strTemp;
		GetDlgItem(ID[i])->GetWindowText(strTemp);
		strTemp.Trim();
		if (strTemp.GetLength() > 0)
		{
			if (i != 0)
			{
				ret += "&";
			}
			ret += strTemp;
		}		
	}
	return ret;
}

void CNetCommDlg::OnBnClickedButtonTest()
{

	

	CDbOperate SqlOPP;
	SqlOPP.OpenSql();
	//��ʱ�ȼٶ��豸�����ߣ�������״̬����Ϊ����״̬
//	SqlOPP.my2UpdateSQL(8, 1);//�����豸�����ߣ�
//	SqlOPP.CloseSql();
//	SqlOPP.InsertSQLPayRecord(1, 79);
/*
	if (SqlOPP.MyQuerySQL(9) == 1)
	{
		SqlOPP.UpdateSQL(9, 9);//
	}
/*
	else
	{
		SqlOPP.InsertSQLPayRecord(9, 7777);
	}
*/
	//��ʱ�ȼٶ��豸�����ߣ�������״̬����Ϊ����״̬
//	SqlOPP.myUpdateSQL(1);//�����豸�����ߣ�

	//SqlOPP.MyQuerySQL(23); AfxMessageBox("2�ɹ�");
//	SqlOPP.MyQuerySQL(245); AfxMessageBox("3�ɹ�");
	//SqlOPP.MyQuerySQL(213); AfxMessageBox("4�ɹ�");
//	SqlOPP.MyQuerySQL(23);

	SqlOPP.CloseSql();
	

	//���Զ�д���ݿ�SQLSERVER2008

	// TODO: �ڴ���ӿؼ�֪ͨ����������
/*
	CDeviceEvent Devent;
	HANDLE hEvent = Devent.GetEvent(6);
	if (hEvent != NULL)
	{
		SetEvent(hEvent);
	}
*/

	if (!::IsWindow(m_dlg.m_hWnd))
	{
		m_dlg.Create(IDD_TEST_DIALOG, NULL);
	}
	m_dlg.ShowWindow(SW_SHOW);
	
}


void CNetCommDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OutputDebug("myTEST  CNetCommDlg::OnTimer  start..");
	CDialogEx::OnTimer(nIDEvent);
	if (nIDEvent == 101)
	{
		if (SQLwriteFlag == 1)
		{
			SQLwriteFlag = 0;

			CDbOperate SqlOPP;
			SqlOPP.OpenSql();

			//��ʼ����ʱ�豸�����ߣ�
			//	SqlOPP.myUpdateSQL(1);//��ʼ�������豸�����ߣ�

			//	SqlOPP.CloseSql();	// TODO: �ڴ���ӿؼ�֪ͨ����������
			//	SqlOPP.MyQuerySQL(23);

		//	AfxMessageBox("100�ɹ�");

			if (SqlOPP.MyQuerySQL(myDTU_ID) == 1)
			{
				//		AfxMessageBox("1�ɹ�");
				SqlOPP.UpdateSQL(myDTU_ID, myID);//
			}
			else
			{
				//		AfxMessageBox("2�ɹ�");
				SqlOPP.InsertSQLPayRecord(myDTU_ID, myID);
			}
			//	AfxMessageBox("3�ɹ�");
			SqlOPP.CloseSql();
			//	AfxMessageBox("4�ɹ�");

		}
		if (SQLwriteFlag2 == 1)
		{
			SQLwriteFlag2 = 0;
			CDbOperate SqlOPP;
			SqlOPP.OpenSql();
			//��ʱ�ȼٶ��豸�����ߣ�������״̬����Ϊ����״̬
		//	AfxMessageBox("200�ɹ�");
			if (SqlOPP.MyQuerySQL(myDTU_ID2) == 1)
			{
				//		AfxMessageBox("1�ɹ�");
			}
			else
			{
				//		AfxMessageBox("1�ɹ�");
				SqlOPP.InsertSQLPayRecord(myDTU_ID2, 0);//���û�и��豸��ͨ����ʱ������Ӹ��豸
			//	AfxMessageBox("201�ɹ�");
			}
		//	AfxMessageBox("202�ɹ�");
			SqlOPP.my2UpdateSQL(myDTU_ID2, 1);//
			SqlOPP.CloseSql();

		}
		if (SQLwriteFlag3 == 1)
		{
			CDbOperate SqlOPP;
			SqlOPP.OpenSql();

			if (SqlOPP.MyQuerySQL(myDTU_ID2) == 1) //�����Ӧ�豸ID��EmpID
			{
				SQLwriteFlag3 = 2;
			//	AfxMessageBox("91�ɹ�");
			}
			else
			{
				SQLwriteFlag3 = 0;
			}
			SqlOPP.CloseSql();
			SQLwriteFlag4 = 0;//֪ͨ�����̣߳�������дSQL
		}


		connectFlgNum++;
		if (connectFlgNum > 400) //��20����
		{
			connectFlgNum = 0;
           


		}
	}
	OutputDebug("myTEST  CNetCommDlg::OnTimer  end..");
}


void CNetCommDlg::OnBnClickedStart2()
{
	CDbOperate SqlOPP;
	SqlOPP.OpenSql();
	OutputDebug("myTEST");
	//��ʼ����ʱ�豸�����ߣ�
	SqlOPP.myUpdateSQL(0);//��ʼ�������豸�����ߣ�

	SqlOPP.CloseSql();

}


void CNetCommDlg::OnBnClickedStart3()
{
	CString str; CString getTIME;
//	CString strSQL;
//	COleDateTime dt = COleDateTime::GetCurrentTime();

//	strSQL.Format("INSERT INTO stuTable(birth) VALUES('%s')", dt.Format("%Y-%m-%d %H:%M:%S"));
	extern CString gettime2();
	unsigned int bb[8];
	int dtu_id = 0;
	int dtu_id_A = 0;
	int dtu_id_B = 0;
	dtu_id = 4560;
	if ((dtu_id >= 3000)&(dtu_id < 7000))
	{
		dtu_id_A = dtu_id / 10; dtu_id_B = dtu_id % 10;
		//tem = MENGsendToDtu(dtu_id_A, dtu_id_B, paymoney, 2); //"2"��֪ͨ�豸���ǻ�Ա����
	}

	float tem3 = 298;
	tem3 = tem3 / 100;


	CString tt1 = "http://127.0.0.1/api/?api_mod=common&extno=100&api_class=notify&api_function=SetYuJingMsg&Set=";
	str.Format("%d", 56); tt1 = tt1 + str;
	tt1 += "&msg=��Һλ";


	CString test_str1 = "��Һλ";
	tt1 += URLEncode(test_str1);
	


	str.Format("%f", tem3);
	CString tt = "http://zizhu.sdxjyjd.com/notify.php";
//	OutputClientString(tt);
	::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
	CDbOperate SqlOPP;
	SqlOPP.OpenSql();

	if (SqlOPP.MyQuerySQL(7003) == 1)
	{
		//		AfxMessageBox("1�ɹ�");

	}
	else
	{
		SqlOPP.InsertSQLPayRecord(7003, 0);//�ǻ�Ա�����ѱ��ֱ�Ӹĳ�0д���
	}
	SqlOPP.my2UpdateSQL(7003, 1);////�ǻ�Ա�����ѱ��ֱ�Ӹĳ�0д���

	bb[1] = 0; bb[2] = 0; bb[3] = 0; bb[4] = 0; bb[5] = 0;
	bb[0] = 0x0203;
	SqlOPP.my3UpdateSQL(56, bb);//



	if (SqlOPP.MyQuerySQL2(53) == 1)
	{ }
	else
	{
				SqlOPP.InsertSQLPayRecord2(53);//���û�и��豸��ͨ����ʱ������Ӹ��豸
	}
	SqlOPP.my3UpdateSQL(53, bb);//
//	SqlOPP.my3UpdateSQL(56, bb);
	//��ʼ����ʱ�豸�����ߣ�
	SqlOPP.myUpdateSQL(1);//��ʼ�������豸�����ߣ�
	
	SqlOPP.CloseSql();	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::CoUninitialize();//����ʼ��COM��
}
