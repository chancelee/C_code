// AddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Chat.h"
#include "AddDlg.h"
#include "afxdialogex.h"


// CAddDlg �Ի���

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDDLG, pParent)
	, m_addEditValue(_T(""))
{

}

CAddDlg::~CAddDlg()
{
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDEDIT, m_addEditValue);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADDFRIENDBTN, &CAddDlg::OnBnClickedAddfriendbtn)
	ON_BN_CLICKED(IDC_CREATEGROUPBTN, &CAddDlg::OnBnClickedCreategroupbtn)
	ON_BN_CLICKED(IDC_JOINGROUPBTN, &CAddDlg::OnBnClickedJoingroupbtn)
//	ON_MESSAGE(WM_USER + 1, &CAddDlg::OnAddGroup)
//ON_MESSAGE(WM_USER + 100, &CAddDlg::OnAddGroup)
END_MESSAGE_MAP()


// CAddDlg ��Ϣ�������

BOOL CAddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CAddDlg::OnBnClickedAddfriendbtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	*	data format:
	*		status:friendname:MYNAME
	*	status:
	*		 0 : add request
	*		 1 : agree
	*		-1 : disagree
	*/
	UpdateData(TRUE);
	if (m_addEditValue.GetLength())
	{
		CStringA data;
		char friendname[20] = { 0 };
		WideCharToMultiByte(CP_ACP, NULL, m_addEditValue, -1, friendname, 20, NULL, FALSE);
		data.Format("%s:%d:%s", g_client.m_nickname, REQUEST, friendname);
		
		g_client.send(ADDFRIEND, data);
		this->EndDialog(IDOK);
	}
}

void CAddDlg::OnBnClickedCreategroupbtn()
{
	/*
	*	data format:
	*		status:friendname:MYNAME
	*
	*/
	UpdateData(TRUE);
	if (m_addEditValue.GetLength())
	{
		CStringA data;
		char groupname[20] = { 0 };
		WideCharToMultiByte(CP_ACP, NULL, m_addEditValue, -1, groupname, 20, NULL, FALSE);
		data.Format("%s:%s", g_client.m_nickname, groupname);

		g_client.send(CREATEGROUP, data);
		this->EndDialog(IDOK);
	}
}


void CAddDlg::OnBnClickedJoingroupbtn()
{
	/*
	*	data format:
	*		status:friendname:MYNAME
	*	status:
	*		 0 : add request
	*		 1 : agree
	*		-1 : disagree
	*/
	UpdateData(TRUE);
	if (m_addEditValue.GetLength())
	{
		CStringA data;
		char groupname[20] = { 0 };
		WideCharToMultiByte(CP_ACP, NULL, m_addEditValue, -1, groupname, 20, NULL, FALSE);
		data.Format("%s:%d:%s", g_client.m_nickname, REQUEST, groupname);

		g_client.send(JOINGROUP, data);
		this->EndDialog(IDOK);
	}
}


//afx_msg LRESULT CAddDlg::OnAddGroup(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


//afx_msg LRESULT CAddDlg::OnAddGroup(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}
