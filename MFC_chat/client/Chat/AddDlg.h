#pragma once
#include "resource.h"
#include "Client.h"
// CAddDlg �Ի���

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
