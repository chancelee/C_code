// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Chat.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "Client.h"
// CChatDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATDLG, pParent)
	, m_inputValue(_T(""))
{

}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATEDIT, m_chatEditCtrl);
	DDX_Text(pDX, IDC_INPUTEDIT, m_inputValue);
	DDX_Control(pDX, IDC_SENDBTN, m_sendBtn);
	DDX_Control(pDX, IDC_SENDBTN, m_sendBtn);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SENDBTN, &CChatDlg::OnBnClickedSendbtn)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CChatDlg 消息处理程序


void CChatDlg::OnBnClickedSendbtn()
{
	/*
	*	data format:
	*		fromname:friendname:data
	*/
	UpdateData(TRUE);
	int size = 0;
	if (size = m_inputValue.GetLength()*2)
	{
		char *name = new char[20]();

		GetWindowTextA(this->m_hWnd, name, 20);
		size += strlen(name);

		char *data = new char[size];
		WideCharToMultiByte(CP_ACP, NULL, m_inputValue, -1, data, size, NULL, FALSE);

		CStringA buf;
		buf.Format("%s:%s:%s",g_client.m_nickname, name, data);

		g_client.send(m_dataType, buf);
		buf.Format("你:\r\n %s\r\n\r\n", data);
		m_chatEditCtrl.SetSel(-1);
		m_chatEditCtrl.ReplaceSel(CString(buf));

		m_inputValue.Empty();
		UpdateData(FALSE);

		delete[] name;
		delete[] data;
	}
}

void CChatDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}
