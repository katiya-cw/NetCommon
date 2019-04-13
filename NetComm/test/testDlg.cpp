// testDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "test.h"
#include "testDlg.h"
#include "MUR500USB.h"
#include "DLL.h"
//#include "VspdCTOMySQL.h"
//#include "../resource.h"
#include "../../MyDefine.h"


int pascal NetComm(char* pServerIP, int nPort, CALL_BACK_GET_DATA callGetBack, CALL_BACK_GET_DATA_GPRS callGetBack2, CALL_BACK_EXCUTE callExcuteBack, CALL_BACK_SHUTDOWN callShutdownBack);
int pascal DownLoadFile(char* pServerIP, int nPort, char* strSaveFile, CALL_BACK_DOWNLOAD callBack);
int pascal myfction(int data1);
int pascal NetSendMyData(int DtuID, unsigned char MepID, unsigned char *pData, int nLen);
int pascal regMYdevice(void);
int pascal NetSendMyDataTrue(int DtuID, unsigned char nMeter_ID, int money, int delayTime);
int pascal NetSendMyData(int DtuID, unsigned char MepID, unsigned char *pData, int nLen);

#define exNetComm NetComm
#define exNmyfction myfction
#define exNetSendMyData NetSendMyData
#define exDownLoadFile DownLoadFile
#define exregMYdevice regMYdevice
#define exNetSendMyDataTrue NetSendMyDataTrue


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
_ConnectionPtr  m_pConnection; 
CString str23;
CString str24;
int tempmy=0;
_RecordsetPtr   m_pRecordset; 
_RecordsetPtr   m_pRecordset1; 
_RecordsetPtr   m_pRecordset2; 
_RecordsetPtr   m_pRecordset3; 

unsigned char ttemp189;
	// char* host="MYSQL������IP";
	char* host="127.0.0.1";
  //  char* host="101.200.204.188";
    char* user="root";
    char* port ="3306";//�˿�û�й�ϵ�������ڲ������κ�
    //char* passwd="�û�����";
	 char* passwd="";
   // char* dbname="���ݿ�����"; 
	char* dbname="mydtu"; 
    char* charset = "GBK";//֧������
    char* Msg = "";//��Ϣ����
//	 VspdCToMySQL * vspdctomysql = new VspdCToMySQL;
	 extern  CString mystingtemp[1000];
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
//	ON_WM_TIMER()
//ON_WM_TIMER()
ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_DISPLY = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
	DDX_Text(pDX, IDC_EDIT2, m_DISPLY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	
unsigned long ttemp, gg,ttemp1,ttemp2;
 CString  mm;double dtemp;

//SetTimer(2,60000,NULL); //��λ�Ǻ���


//CString dmm="122.114.55.116";
#ifdef GaiDongIPaddress
	 CString dmm = "127.0.0.1";
#else
 CString dmm = "101.200.204.188";
#endif
	
SetDlgItemText(IDC_EDIT1,dmm);
gg=8001;
    dmm.Format("%d",gg);
SetDlgItemText(IDC_EDIT3,dmm);


    gg=99;
    mm.Format("%d",gg);
    SetDlgItemText(IDC_EDIT4,mm);
	
     gg=1;
    mm.Format("%d",gg);
    SetDlgItemText(IDC_EDIT5,mm);

     gg=1;
    mm.Format("%d",gg);
    SetDlgItemText(IDC_EDIT7,mm);

	dtemp=68.54;
    mm.Format("%f",dtemp);
    SetDlgItemText(IDC_EDIT6,mm);

int i = 0; unsigned char mybuf[32];
char * Ip = "122.114.55.116";
 int Comm = 2000;
//���ݿ�����-----------START---------


   

 //���ݿ�����-----------END-------
 //----------------��ʼ����------START----------



 CString tt1;
 GetDlgItemText(IDC_EDIT3, tt1);
 Comm = atol(tt1);
 GetDlgItemText(IDC_EDIT1, tt1);
 //	Ip=atol(tt1);
 Ip = (LPSTR)(LPCSTR)tt1;

 //���ݿ�����-----------START---------

 /*
 if (vspdctomysql->ConnMySQL(host, port, dbname, user, passwd, charset, Msg) == 0)
 {
	  //  AfxMessageBox("MYSQL���ݿ����ӳɹ�");
 }
 else
 {
	  //  AfxMessageBox(Msg);
 }
 */

 //���ݿ�����-----------END-------


 //SetTimer(1, 500, NULL); //��λ�Ǻ���

						 //CALL_BACK_GET_DATA
 CALL_BACK_GET_DATA callBack_GetData;
 CALL_BACK_EXCUTE  callBack_EXCUTE;
 CALL_BACK_GET_DATA_GPRS callBack_GPRS;
 CALL_BACK_SHUTDOWN callBack_ShutDown;

 callBack_GetData = callBack_GetDataFunction;
 callBack_EXCUTE = callBack_EXCUTEFunction;
 callBack_ShutDown = callBack_ShutDownFunction;
 callBack_GPRS = callBack_GPRSFunction;

 UpdateData(true);
 //start run your function
// int mm = 5;
 //i = exNmyfction(mm);

// mm = 8;

// i = exregMYdevice();
 // i = exNetComm(Ip, Comm, callBack_GetData,callBack_GPRS, callBack_EXCUTE, callBack_ShutDown);



 try
 {

	 i = exNetComm(Ip, Comm, callBack_GetData, callBack_GPRS, callBack_EXCUTE, callBack_ShutDown);
 }
 catch (_com_error *e)
 {

 }
 UpdateData(false);
 //FreeLibrary(hdll);

 if (i == 0)
 {
	 // timer1.Enabled = false;
	// MessageBox("�Ѿ�����");
 }
 else
 {
	 // timer1.Enabled = true;
	 // toolStripStatusLink.Text = "����....������";
	 MessageBox("����....���������߼��������Ƿ��?");
 }






	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}








void CTestDlg::OnButton1() 
{int i = 0; unsigned char mybuf[32];
char * Ip ;
 int Comm ;

CString tt1;
  GetDlgItemText(IDC_EDIT3,tt1);
	Comm=atol(tt1);
 GetDlgItemText(IDC_EDIT1,tt1);
//	Ip=atol(tt1);
 Ip=(LPSTR)(LPCSTR)tt1;

//���ݿ�����-----------START---------


    //��ʼ��

 /*
 if (vspdctomysql->ConnMySQL(host, port, dbname, user, passwd, charset, Msg) == 0)
 {
	 //   AfxMessageBox("MYSQL���ݿ����ӳɹ�");
 }
    else
	{
        //   AfxMessageBox(Msg);
	}
 */


 //���ݿ�����-----------END-------

//memset(MYbb,0,32);//����ǰ����ս��ջ�����
// SetTimer(1,500,NULL); //��λ�Ǻ���

//CALL_BACK_GET_DATA
  CALL_BACK_GET_DATA callBack_GetData;
  CALL_BACK_EXCUTE  callBack_EXCUTE;
  CALL_BACK_GET_DATA_GPRS callBack_GPRS;
  CALL_BACK_SHUTDOWN callBack_ShutDown;

  callBack_GetData = callBack_GetDataFunction;
  callBack_EXCUTE = callBack_EXCUTEFunction; 
  callBack_ShutDown = callBack_ShutDownFunction;
  callBack_GPRS = callBack_GPRSFunction;
#if 0 //pj
HINSTANCE hdll; 

MyNetComm exNetComm;
Nmyfction exNmyfction;
MYNetSendMyData exNetSendMyData;
MYDownLoadFile  exDownLoadFile;
MYregMYdevice exregMYdevice;


hdll=LoadLibrary("NetComm.dll ");
if(hdll!=NULL)
{
exNetComm=(MyNetComm)GetProcAddress(hdll,"NetComm");
exNmyfction=(Nmyfction)GetProcAddress(hdll,"myfction");
exNetSendMyData=(MYNetSendMyData)GetProcAddress(hdll,"NetSendMyData");

exDownLoadFile=(MYDownLoadFile)GetProcAddress(hdll,"DownLoadFile");
exregMYdevice=(MYregMYdevice)GetProcAddress(hdll,"regMYdevice");
}
else
{
AfxMessageBox("�޷�����DLL---NetComm.dll");
return;


}
#endif
UpdateData(true);
//start run your function
 int mm=5;
i=exNmyfction(mm);

mm=8;

i=exregMYdevice();
 // i = exNetComm(Ip, Comm, callBack_GetData,callBack_GPRS, callBack_EXCUTE, callBack_ShutDown);



            try
            {

  i = exNetComm(Ip, Comm, callBack_GetData,callBack_GPRS, callBack_EXCUTE, callBack_ShutDown);
            }
            catch(_com_error *e) 
            {

            }
UpdateData(false);
//FreeLibrary(hdll);

            if (i == 0)
            {
               // timer1.Enabled = false;
                	MessageBox("�Ѿ�����"); 
            }
            else
            {
               // timer1.Enabled = true;
               // toolStripStatusLink.Text = "����....������";
				MessageBox("����....���������߼��������Ƿ��?"); 
            }

         //   memset(mybuf,11,32);

           //i = exNetSendMyData(19,1,mybuf,32);
         

//FreeLibrary(hdll);




}



void CTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
unsigned long ttemp, gg,ttemp1,ttemp2;
 CString  mm;double dtemp;int i;
//memset(MYbb,0,32);
 UpdateData(true);

 for(i=0;i<10;i++)
 {
//mm.Format("%d",MYbb[i]);
    m_DISPLY+=mm;
	m_DISPLY+="-";
mm="";
 }
m_DISPLY+="\n";m_DISPLY+="\n";
 for(i=10;i<20;i++)
 {
//mm.Format("%d",MYbb[i]);
    m_DISPLY+=mm;m_DISPLY+="-";
mm="";
 }
m_DISPLY+="\n";
m_DISPLY+="\n";

 for(i=20;i<32;i++)
 {
//mm.Format("%d",MYbb[i]);
    m_DISPLY+=mm;m_DISPLY+="-";
mm="";
 }
m_DISPLY+="\n";m_DISPLY+="\n";



 UpdateData(false);
   SetDlgItemText(IDC_EDIT2,mm);
}

void CTestDlg::OnButton3() 
{
	
}

void CTestDlg::OnButton4() 
{  unsigned int Down_Data[30];
   unsigned int CheckBox[2];
	int i; 
	for (i = 0; i < 30; i++)
	{
		Down_Data[i] = i;

	}

	::CoInitialize(NULL);//��ʼ��COM�� �߳��ڶ�дSQL�����ʼ���ͷ���ʼ�������򾭳�����SQL��д����
	CDbOperate SqlOPP;
	SqlOPP.OpenSql();
	if (SqlOPP.myUpdateDown_DataSQL(8006, Down_Data, CheckBox) == true)
	{
		//		AfxMessageBox("1�ɹ�");
		i = 9;
	}
	for (i = 0; i < 30; i++)
	{
		Down_Data[i] = 0;

	}
	if (SqlOPP.MyQueryDown_DataSQL(8006, Down_Data, CheckBox) == 1)
	{
		i = 8;
	}
	//	AfxMessageBox("3�ɹ�");
	SqlOPP.CloseSql();
	::CoUninitialize();//����ʼ��COM��

	



}

void CTestDlg::OnButton5() 
{

}



void CTestDlg::OnButton6() 
{

	
}

void CTestDlg::OnButton8()
{
}

void CTestDlg::OnButton9() 
{
	

}

void CTestDlg::OnButton7() 
{

}

void CTestDlg::OnButton10() 
{
}

void CTestDlg::OnButton11() 
{ 
}

void CTestDlg::OnButton12() 
{
	int i = 0; unsigned char mybuf[32];

	unsigned long ttemp, ttemp1, ttemp2, ttemp3;
	CString  tt1;
//	memset(MYbb, 0, 32);//����ǰ����ս��ջ�����
#if 0 //pj
	HINSTANCE hdll;

	MyNetComm exNetComm;
	Nmyfction exNmyfction;
	MYNetSendMyDataTrue exNetSendMyDataTrue;

	hdll = LoadLibrary("NetComm.dll ");
	if (hdll != NULL)
	{

		exNetSendMyDataTrue = (MYNetSendMyDataTrue)GetProcAddress(hdll, "NetSendMyDataTrue");

	}
	else
	{
		AfxMessageBox("�޷�����DLL---NetComm.dll");
		return;


	}
#endif
	UpdateData(true);
	//start run your function




	memset(mybuf, 11, 32);

	try
	{
		GetDlgItemText(IDC_EDIT4, tt1);
		ttemp2 = atol(tt1);

		GetDlgItemText(IDC_EDIT5, tt1);
		ttemp189 = atol(tt1);

		GetDlgItemText(IDC_EDIT7, tt1);
		ttemp3 = atol(tt1);


		GetDlgItemText(IDC_EDIT6, tt1);

		float  ttemp88;
		ttemp88 = atof(tt1); ttemp1 = ttemp88 * 100;



		myREVflag = 0;//�ж�ʲôʱ����յ����ݣ�����1


		i = exNetSendMyDataTrue(ttemp2, ttemp189, ttemp1, ttemp3);//���豸��Ϊ19�ĵ�ַ����56.89Ԫ	           

		if (i == 45)//f���ͳɹ�
		{
		}


	}
	catch (_com_error *e)
	{

	}

	UpdateData(false);
	//FreeLibrary(hdll);	


}
//DevID:�豸����ַ
//Dev_id�豸�ӵ�ַ
unsigned char MENGsendToDtu(unsigned long DevID,unsigned char Dev_id, unsigned long money, unsigned long deyTime)
{
	int i = 0;// unsigned char mybuf[32];

	unsigned long ttemp, ttemp1, ttemp2, ttemp3;
	unsigned char ttemp189;
	CString  tt1;
	//	memset(MYbb, 0, 32);//����ǰ����ս��ջ�����


	//start run your function

	//memset(mybuf, 11, 32);
	OutputDebug("myTEST  MENGsendToDtu()  start..");

	try
	{
		/*
		ttemp189 = atol(tt1);
		ttemp3 = atol(tt1);
		GetDlgItemText(IDC_EDIT6, tt1);
		float  ttemp88;
		ttemp88 = atof(tt1); ttemp1 = ttemp88 * 100;
		*/
		myREVflag = 0;//�ж�ʲôʱ����յ����ݣ�����1


		i = exNetSendMyDataTrue(DevID, Dev_id, money, deyTime);//���豸��Ϊ19�ĵ�ַ����56.89Ԫ	           

		if (i == 45)//f���ͳɹ�
		{
			return 1;
		}
		else
		{
			return 0;
		}


	}
	catch (_com_error *e)
	{

	}
	OutputDebug("myTEST  MENGsendToDtu()   end..");

	//FreeLibrary(hdll);	
}


void SendLoop()
{
	
}//���ʹ����߼�
void  RevLoop()
{
	
}//���մ����߼�



void CTestDlg::OnTimer(UINT nIDEvent) 
{

}







void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	
}
