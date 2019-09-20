#pragma once
#include "afxwin.h"
#include "resource.h"
#include "Client.h"

// CLogDlg 对话框

class CLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogDlg)

public:
	CLogDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_nickNameValue;
	CString m_passwordValue;
	CButton m_regCtrl;
	CButton m_passwordCtrl;
	afx_msg void OnBnClickedRegbtn();
	afx_msg void OnBnClickedLogbtn();
	virtual BOOL OnInitDialog();
};

DWORD WINAPI logRecvProc(LPVOID arg);
