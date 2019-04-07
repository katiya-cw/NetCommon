// DialogReg.cpp : implementation file
//

#include "stdafx.h"
#include "netcomm.h"
#include "DialogReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogReg dialog
#include "Des.h"


CDialogReg::CDialogReg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogReg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogReg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bOK = FALSE;
	m_bNoSn = FALSE;
}


void CDialogReg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogReg)
	DDX_Control(pDX, IDC_BUTTON_TEMP, m_buTemp);
	DDX_Control(pDX, IDC_EDIT_TIME, m_edTime);
	DDX_Control(pDX, IDC_EDIT_CODE, m_edCode);
	DDX_Control(pDX, IDC_EDIT_KEY, m_edKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogReg, CDialog)
	//{{AFX_MSG_MAP(CDialogReg)
	ON_BN_CLICKED(IDC_BUTTON_TEMP, OnButtonTemp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogReg message handlers

BOOL CDialogReg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);	
	::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);  
	
	GetKey();
	if(!m_bNoSn)
	{
		m_buTemp.EnableWindow(FALSE);

		CString strKey;
		m_edKey.GetWindowText(strKey);
		
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
					MessageBox("注册码错误.");
					return 0;
				}
				CString strY,strM,strD;
				
				strY = strSn.Left(4);
				strM = strSn.Mid(4,2);
				strD = strSn.Mid(6,2);
				
				if(atoi(strY) == 1 && atoi(strM) == 2 && atoi(strD) == 3)
				{
					m_edTime.SetWindowText("无限制版本");
				}
				else
				{
					m_edTime.SetWindowText(strY + "-" + strM + "-" + strD);
				}
				
			}
		}
	}
	else
	{
		m_buTemp.EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogReg::MaskRem(CString strKey,CString strData,CString & strNew)
{
	CDES des;
	des.InitializeKey(strKey,TRUE);
	des.DecryptAnyLength(strData,TRUE,strNew);
	
	//	strNew.Format("%s",des.GetPlaintextAnyLength());
}

void CDialogReg::GetKey()
{
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
			CString s;
			
			s.Format("%X",ID1 * 3);
			m_edKey.SetWindowText(s);
		}
		//m_edKey.SetWindowText()
	}
}

void CDialogReg::OnOK() 
{
	// TODO: Add extra validation here
	GetKey();
	
	CString strKey;
	m_edKey.GetWindowText(strKey);
	
	CString strSn;
	m_edCode.GetWindowText(strSn);
	
	CString strY,strM,strD,strNew;
	
	MaskRem(strKey,strSn,strNew);
	if(strNew.GetLength() != 8)
	{
		MessageBox("注册码错误.");
		return ;
	}
	strY = strNew.Left(4);
	strM = strNew.Mid(4,2);
	strD = strNew.Mid(6,2);
	
	if(atoi(strY) == 1 && atoi(strM) == 2 && atoi(strD) == 3)
	{
		m_edTime.SetWindowText("无限制版本");
	}
	else
	{
		m_edTime.SetWindowText(strY + "-" + strM + "-" + strD);
	}
	
	//Save
	
	char sys[MAX_PATH] = {0};
	GetSystemDirectory(sys,MAX_PATH);
	
	CString strFile;
	strFile.Format("%s\\s1n1.dat",sys);//Reg sn
	
	
	CStdioFile file;
	if(file.Open(strFile,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate))
	{
		char strBuff[1024] = {0};
		memcpy(strBuff,strSn,strSn.GetLength());
		
		file.Write(strBuff,strSn.GetLength());
		
		file.Close();

		m_bOK = TRUE;
	}
	else
	{
		AfxMessageBox("Sn write error.");
		m_bOK = FALSE;
	}
}

void CDialogReg::MaskAdd(CString strKey,CString strData,CString & strNew)
{
	CDES des;
	des.InitializeKey(strKey,TRUE);
	des.EncryptAnyLength(strData,TRUE,strNew);
	
	//strNew.Format("%s",des.GetCiphertextAnyLength());
}

void CDialogReg::OnButtonTemp() 
{
	// TODO: Add your control notification handler code here
	if(m_bNoSn)
	{
		CString key,day,sn;
		m_edKey.GetWindowText(key);
		
		long t = CTime::GetCurrentTime().GetTime() + 3600 * 24 * 7;

		CTime time(t);
		
		int y,m,d;
		y = time.GetYear(),m = time.GetMonth(),d = time.GetDay();
		CString strM;
		day.Format("%04d%02d%02d",y,m,d);
		
		MaskAdd(key,day,strM);
		
		m_edCode.SetWindowText(strM);

	}
	else
	{
		AfxMessageBox("操作无效");
	}
}
