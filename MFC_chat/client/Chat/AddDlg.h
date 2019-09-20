#pragma once
#include "resource.h"
#include "Client.h"
// CAddDlg 对话框

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_addEditValue;
	afx_msg void OnBnClickedAddfriendbtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCreategroupbtn();
	afx_msg void OnBnClickedJoingroupbtn();
protected:
//	afx_msg LRESULT OnAddGroup(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnAddGroup(WPARAM wParam, LPARAM lParam);
};
