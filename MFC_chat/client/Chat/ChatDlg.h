#pragma once
#include "afxwin.h"


// CChatDlg 对话框

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_friendname;
	CEdit m_chatEditCtrl;
	CString m_inputValue;
	CButton m_sendBtn;
	enum DATATYPE m_dataType;
	afx_msg void OnBnClickedSendbtn();
	afx_msg void OnClose();
};
