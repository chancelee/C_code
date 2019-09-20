#pragma once
#include "afxwin.h"


// CChatDlg �Ի���

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
