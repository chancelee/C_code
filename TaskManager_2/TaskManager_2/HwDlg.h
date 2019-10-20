#pragma once


/*
*	Functions of this class:
*		Display hardware infomations;
*		Optimization.
*/


// CCpuDlg 对话框

class CHwDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHwDlg)

public:
	CHwDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHwDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_cProcValue;
	CString m_pageSizeValue;
	CString m_cpuTypeValue;
	CString m_phyMemValue;
	CString m_virtValue;
	virtual BOOL OnInitDialog();

	/*
	*	Optimization button
	*/
	afx_msg void OnBnClickedBtnoptimize();
};
