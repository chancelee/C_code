
// TaskManager_2Dlg.h : ͷ�ļ�
//


/*
*	This is the main window implemented by tab control:
*		There is a status bar to show the cpu infomation.
*		Hot key "alt + k" is registered.
*		Elevation is also implemented.
*/

#pragma once
#include "afxcmn.h"


// CTaskManager_2Dlg �Ի���
class CTaskManager_2Dlg : public CDialogEx
{
// ����
public:
	CTaskManager_2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMANAGER_2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
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
