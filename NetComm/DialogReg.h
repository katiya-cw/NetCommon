#if !defined(AFX_DIALOGREG_H__8190D58E_30B8_4228_8737_34B79C83C238__INCLUDED_)
#define AFX_DIALOGREG_H__8190D58E_30B8_4228_8737_34B79C83C238__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogReg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogReg dialog

class CDialogReg : public CDialog
{
// Construction
public:
	CDialogReg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogReg)
	enum { IDD = IDD_DIALOG_REG };
	CButton	m_buTemp;
	CEdit	m_edTime;
	CEdit	m_edCode;
	CEdit	m_edKey;
	//}}AFX_DATA

	BOOL m_bNoSn;//无注册码，可以试用
	void MaskAdd(CString strKey,CString strData,CString & strNew);

	void GetKey();
	void MaskRem(CString strKey,CString strData,CString & strNew);

	BOOL m_bOK;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogReg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogReg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonTemp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGREG_H__8190D58E_30B8_4228_8737_34B79C83C238__INCLUDED_)
