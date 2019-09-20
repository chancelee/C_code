#pragma once

#include <vector>

#include "ChatDlg.h"

#include "afxcmn.h"
#include "resource.h"
#include "Client.h"

#define WM_ADDFRIEND WM_USER+100


// CFriendDlg �Ի���

class CFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendDlg)

public:
	CFriendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFriendDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRIENDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int cFriend;
	std::vector<CChatDlg *> m_vChatDlg;
	CListCtrl m_friendListCtrl;

	virtual BOOL OnInitDialog();
	void addFriend(const char* friendname);
	void recvFriendData(DataPack *pDataPack);


	/*
	*	������ѣ��������촰��
	*/
	afx_msg void OnClickFriendlist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnAddfriend(WPARAM wParam, LPARAM lParam);
};
