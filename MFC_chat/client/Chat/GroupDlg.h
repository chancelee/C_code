#pragma once

#include <vector>
#include "resource.h"
#include "afxcmn.h"
#include "ChatDlg.h"
#include "Client.h"

#define WM_ADDGROUP WM_USER+200

// CGroupDlg 对话框

class CGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupDlg)

public:
	CGroupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int cGroup;
	std::vector<CChatDlg *> m_vChatDlg;
	CListCtrl m_groupListCtrl;

	/*
	*	点击群聊，弹出群聊窗口。
	*/
	afx_msg void OnClickGrouplist(NMHDR *pNMHDR, LRESULT *pResult);
	//void addGroup(const char* groupname);

	virtual BOOL OnInitDialog();

	/*
	*	收到群聊消息
	*	参数：数据包结构体
	*/
	void recvGroupData(DataPack *pDataPack);
protected:
	afx_msg LRESULT OnAddgroup(WPARAM wParam, LPARAM lParam);
};
