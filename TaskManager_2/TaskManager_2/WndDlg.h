#pragma once
#include "afxcmn.h"
#include "Tool.h"

/*
*	Function of this class:
*		Enum windows 
*		Flush.
*/

// CWndDlg 对话框

class CWndDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWndDlg)

public:
	CWndDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWndDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WNDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_wndListCtrl;

	void updateWindowList();
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickWndlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFlushFlush();
};
