#pragma once

#include <vector>
#include "resource.h"
#include "afxcmn.h"
#include "ChatDlg.h"
#include "Client.h"

#define WM_ADDGROUP WM_USER+200

// CGroupDlg �Ի���

class CGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupDlg)

public:
	CGroupDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGroupDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int cGroup;
	std::vector<CChatDlg *> m_vChatDlg;
	CListCtrl m_groupListCtrl;

	/*
	*	���Ⱥ�ģ�����Ⱥ�Ĵ��ڡ�
	*/
	afx_msg void OnClickGrouplist(NMHDR *pNMHDR, LRESULT *pResult);
	//void addGroup(const char* groupname);

	virtual BOOL OnInitDialog();

	/*
	*	�յ�Ⱥ����Ϣ
	*	���������ݰ��ṹ��
	*/
	void recvGroupData(DataPack *pDataPack);
protected:
	afx_msg LRESULT OnAddgroup(WPARAM wParam, LPARAM lParam);
};
