
// NetCommDlg.h : 头文件
//

#pragma once
#include "test/testDlg.h"
#include "httpserver.h"

struct TREAD_INFO
{
	int port;
	ULONG ip;
};

// CNetCommDlg 对话框
class CNetCommDlg : public CDialogEx
{
// 构造
public:
	CNetCommDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETCOMM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	//显示服务器参数
	void ShowServerParam(CString param);
	//获取客户输入参数
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
