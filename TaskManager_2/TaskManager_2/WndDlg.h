#pragma once
#include "afxcmn.h"
#include "Tool.h"

/*
*	Function of this class:
*		Enum windows 
*		Flush.
*/

// CWndDlg �Ի���

class CWndDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWndDlg)

public:
	CWndDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWndDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WNDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_wndListCtrl;

	void updateWindowList();
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickWndlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFlushFlush();
};
