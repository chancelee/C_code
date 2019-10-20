
// TaskManager_2Dlg.h : 头文件
//


/*
*	This is the main window implemented by tab control:
*		There is a status bar to show the cpu infomation.
*		Hot key "alt + k" is registered.
*		Elevation is also implemented.
*/

#pragma once
#include "afxcmn.h"


// CTaskManager_2Dlg 对话框
class CTaskManager_2Dlg : public CDialogEx
{
// 构造
public:
	CTaskManager_2Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMANAGER_2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	/*
	*	Tab control
	*/
	CTabCtrl m_tabCtrl;
	CDialogEx *m_tabs[6];
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowTabWnd(int index);

	/*
	*	  Status bar will show the cpu rate and free phyMem 
	*	and flush according to the timer.
	*/
	CStatusBar m_status;
	void OnTimer(UINT_PTR nIDEvent);
	void CTaskManager_2Dlg::flushStatus();

	/*
	*	HotKey virtual function.
	*/
	BOOL m_bIsWindowHide;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
