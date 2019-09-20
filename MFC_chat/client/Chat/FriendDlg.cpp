// FriendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "FriendDlg.h"

// CFriendDlg 对话框

IMPLEMENT_DYNAMIC(CFriendDlg, CDialogEx)

CFriendDlg::CFriendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FRIENDDLG, pParent)
{

}

CFriendDlg::~CFriendDlg()
{
}

void CFriendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRIENDLIST, m_friendListCtrl);
}


BEGIN_MESSAGE_MAP(CFriendDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_FRIENDLIST, &CFriendDlg::OnClickFriendlist)
	ON_MESSAGE(WM_ADDFRIEND, &CFriendDlg::OnAddfriend)
END_MESSAGE_MAP()

void CFriendDlg::recvFriendData(DataPack *pDataPack)
{
	/*
	*	pDataPack->head
	*		SENDTOFRIEND, size
	*	pDataPack->data format:
	*		fromname:data
	*/
	char *pFriendname = NULL, *pData = NULL;
	CStringA buf;

	std::vector<CChatDlg*>::iterator it;

	pFriendname = strtok_s(pDataPack->data, ":", &pData);
	if (!pFriendname || !pData)
	{
		return;
	}

	buf.Format("%s:\r\n  %s\r\n\r\n", pFriendname, pData);
	for (int i = 0; i < cFriend; ++i)
	{
		if (!m_vChatDlg[i]->m_friendname.Compare(CString(pFriendname)))
		{
			m_friendListCtrl.SetItemText(i, 1, CString("有新消息"));
			m_vChatDlg[i]->m_chatEditCtrl.SetSel(-1);
			m_vChatDlg[i]->m_chatEditCtrl.ReplaceSel(CString(buf));
			break;
		}
	}
}

void CFriendDlg::addFriend(const char* friendname)
{
	m_friendListCtrl.InsertItem(cFriend, L"");
	WCHAR buf[20] = { 0 };
	MultiByteToWideChar(CP_ACP, NULL, friendname, -1, buf, _countof(buf));
	m_friendListCtrl.SetItemText(cFriend, 0, buf);
	++cFriend;
}

// CFriendDlg 消息处理程序



BOOL CFriendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_friendListCtrl.InsertColumn(0, L"", LVCFMT_CENTER, 100);
	m_friendListCtrl.InsertColumn(1, L"", LVCFMT_CENTER, 100);

	cFriend = 0;
	return TRUE;  // return TRUE unless you set the focus to a contro
				  // 异常: OCX 属性页应返回 FALSE
}

void CFriendDlg::OnClickFriendlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMItemActivate->iItem > -1 && pNMItemActivate->iSubItem == 0)
	{
		m_vChatDlg[pNMItemActivate->iItem]->ShowWindow(SW_SHOW);
		m_friendListCtrl.SetItemText(pNMItemActivate->iItem, 1, CString(""));
	}
	/*CString friendname = m_friendListCtrl.GetItemText(pNMItemActivate->iItem, 0);

	if (friendname.GetLength())
	{
		CChatDlg *pChatDlg = new CChatDlg;
		m_vChatDlg.push_back(pChatDlg);

		pChatDlg->Create(IDD_CHATDLG);
		pChatDlg->m_friendname = friendname;
		pChatDlg->SetWindowTextW(friendname);
		pChatDlg->ShowWindow(SW_SHOWNORMAL);
	}*/
}


afx_msg LRESULT CFriendDlg::OnAddfriend(WPARAM wParam, LPARAM lParam)
{
	char *friendname = (char*)wParam;
	m_friendListCtrl.InsertItem(cFriend, L"");
	WCHAR buf[20] = { 0 };
	MultiByteToWideChar(CP_ACP, NULL, friendname, -1, buf, _countof(buf));
	m_friendListCtrl.SetItemText(cFriend, 0, buf);
	++cFriend;

	CChatDlg *pChatDlg = new CChatDlg;
	pChatDlg->m_dataType = SENDTOFRIEND;
	m_vChatDlg.push_back(pChatDlg);

	pChatDlg->m_friendname = friendname;
	pChatDlg->Create(IDD_CHATDLG);
	pChatDlg->SetWindowTextW(CString(friendname));
	pChatDlg->ShowWindow(SW_HIDE);
	//pChatDlg->UpdateWindow();
	return 0;
}
