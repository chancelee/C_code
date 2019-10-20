#pragma once
#include <windows.h>
#include "afxcmn.h"
#include "Tool.h"

/*
*	Show process threads and modules.
*/

#define THREAD	0
#define MODULE	1

// ProcessSubDialog 对话框

class ProcessSubDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProcessSubDialog)

public:
	ProcessSubDialog(CWnd* pParent = NULL);   // 标准构造函数
	ProcessSubDialog(int type, CString strCap, DWORD pid, CWnd* pParent = NULL);
	virtual ~ProcessSubDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSSUBDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_processSubListCtrl;
	CString m_strCap;
	int m_type;
	DWORD m_pid;

	std::vector<MYMODULEINFO> m_vModules;
	std::vector<THREADENTRY32> m_vThreads;

	void AddModules();
	void AddThreads();

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
