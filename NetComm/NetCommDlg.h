
// NetCommDlg.h : ͷ�ļ�
//

#pragma once
#include "test/testDlg.h"
#include "httpserver.h"

struct TREAD_INFO
{
	int port;
	ULONG ip;
};

// CNetCommDlg �Ի���
class CNetCommDlg : public CDialogEx
{
// ����
public:
	CNetCommDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETCOMM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnServerInfoMsg(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnClientInfoMsg(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnServerURLMsg(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListServer();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedButtonSend();
private:
	//��ʾ����������
	void ShowServerParam(CString param);
	//��ȡ�ͻ��������
	CString GetClientParam();
private:
	TREAD_INFO m_threadInfo;
public:
	afx_msg void OnBnClickedButtonTest();
private:
	CTestDlg m_dlg;
	CHttpServer m_server;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStart2();
	afx_msg void OnBnClickedStart3();
};
