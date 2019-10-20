#pragma once
#include <vector>
#include "afxcmn.h"
#include "Tool.h"

/*
*	Functions of this class:
*		Enum process information;
*		Show threads in sub dialog;
*		Show modules in sub dialog;
*		Kill process;
*		Flush;
*		Protect this manager from beging killed by os manager.
*/

// CProcessDlg 对话框

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	std::vector<PROCESSINFO> m_vProcess;

	virtual BOOL OnInitDialog();
	BOOL UpdataProcessList();
	//void SetItem(std::vector<PROCESSINFO> processList);

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	/*
	*	Flush
	*/
	afx_msg void OnProcessFlush();

	/*
	*	Kill process.
	*/
	afx_msg void OnProcessKillprocess();

	/*
	*	Show threads.
	*/
	afx_msg void OnProcessThread();


	/*
	*	Show modules.
	*/
	afx_msg void OnProcessModule();



	/*
	*	Protect this manager from beging killed by os manager.
	*/
	afx_msg void OnProcessProtectall();
};
