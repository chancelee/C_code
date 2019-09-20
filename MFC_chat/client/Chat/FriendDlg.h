#pragma once

#include <vector>

#include "ChatDlg.h"

#include "afxcmn.h"
#include "resource.h"
#include "Client.h"

#define WM_ADDFRIEND WM_USER+100


// CFriendDlg 对话框

class CFriendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendDlg)

public:
	CFriendDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFriendDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRIENDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int cFriend;
	std::vector<CChatDlg *> m_vChatDlg;
	CListCtrl m_friendListCtrl;

	virtual BOOL OnInitDialog();
	void addFriend(const char* friendname);
	void recvFriendData(DataPack *pDataPack);


	/*
	*	点击好友，弹出聊天窗口
	*/
	afx_msg void OnClickFriendlist(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnAddfriend(WPARAM wParam, LPARAM lParam);
};
