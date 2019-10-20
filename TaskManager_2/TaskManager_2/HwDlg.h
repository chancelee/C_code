#pragma once


/*
*	Functions of this class:
*		Display hardware infomations;
*		Optimization.
*/


// CCpuDlg �Ի���

class CHwDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHwDlg)

public:
	CHwDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHwDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
