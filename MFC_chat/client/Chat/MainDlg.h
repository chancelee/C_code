
// ChatDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Client.h"

// CChatDlg 对话框
class CMainDlg : public CDialogEx
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINDLG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCtrl;
	CDialogEx *m_tabWnds[2] = { 0 };
	CString m_curChatValue;

	/*
	*	tab子窗口显示
	*/
	void showTabWnd(int index);
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	/*
	*	添加好友或群聊按钮
	*/
	afx_msg void OnBnClickedAddbtn();
	/*
	*	3个处理好友消息的函数
	*/

	/*
	*	在好友列表中显示好友
	*	参数：数据包结构体
	*/
	void addFriend(DataPack *pDataPack);
	
	/*
	*	同意/拒绝好友请求
	*	参数：数据包结构体
	*/
	void agreeFriend(DataPack *pDataPack);
	
	/*
	*	接收好友消息
	*	参数：数据包结构体
	*/
	void recvFriendData(DataPack *pDataPack);
//////////////////////////////////////////////////////////////////////
	/*
	*	3个处理群聊消息的函数
	*/
	/*
	*	同意/拒绝加入群聊
	*	参数：数据包结构体
	*/
	void agreeJoinGroup(DataPack * pDataPack);

	///*
	//*	群聊列表中显示群聊
	//*	参数：数据包结构体
	//*/
	//void addGroup(DataPack * pDataPack);

	/*
	*	接收群聊消息
	*	参数：数据包结构体
	*/
	void recvGroupData(DataPack *pDataPack);

	
};

/*
*	服务器消息接收线程
*/
DWORD WINAPI chatRecvProc(LPVOID arg);



