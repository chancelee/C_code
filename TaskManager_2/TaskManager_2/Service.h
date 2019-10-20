#pragma once
#include <Winsvc.h>
#include "afxcmn.h"

/*
*	Functions of this class:
*		Enum services;
*		Start service;
*		Stop service;
*		Flush.
*/


// CService �Ի���

class CService : public CDialogEx
{
	DECLARE_DYNAMIC(CService)

public:
	CService(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CService();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	SC_HANDLE m_hScManager;
	LPENUM_SERVICE_STATUS_PROCESS m_pEnumService;
	virtual BOOL OnInitDialog();

	void enumServices();
	afx_msg void OnServiceStart();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnServiceFlush();
	afx_msg void OnServiceStop();
};
