// GroupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GroupDlg.h"
#include "afxdialogex.h"


// CGroupDlg 对话框

IMPLEMENT_DYNAMIC(CGroupDlg, CDialogEx)

CGroupDlg::CGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GROUPDLG, pParent)
{

}

CGroupDlg::~CGroupDlg()
{
}

void CGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUPLIST, m_groupListCtrl);
}


BEGIN_MESSAGE_MAP(CGroupDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_GROUPLIST, &CGroupDlg::OnClickGrouplist)
	ON_MESSAGE(WM_ADDGROUP, &CGroupDlg::OnAddgroup)
END_MESSAGE_MAP()


void CGroupDlg::recvGroupData(DataPack *pDataPack)
{
	/*
	*	pDataPack->head
	*		SENDTOFRIEND, size
	*	pDataPack->data format:
	*		fromname:data
	*/
	char *pFriendname = NULL, *pGroupname, *pData = NULL;
	CStringA buf;
	
	std::vector<CChatDlg*>::iterator it;

	pFriendname = strtok_s(pDataPack->data, ":", &pGroupname);
	pGroupname = strtok_s(pGroupname, ":", &pData);
	if (!pFriendname || !pGroupname || !pData )
	{
		return;
	}

	buf.Format("%s:\r\n  %s\r\n\r\n", pFriendname, pData);
	for (int i = 0; i < cGroup; ++i)
	{
		if (!m_vChatDlg[i]->m_friendname.Compare(CString(pGroupname)))
		{
			m_groupListCtrl.SetItemText(i, 1, CString("有新消息"));
			m_vChatDlg[i]->m_chatEditCtrl.SetSel(-1);
			m_vChatDlg[i]->m_chatEditCtrl.ReplaceSel(CString(buf));
			break;
		}
	}


}

BOOL CGroupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_groupListCtrl.InsertColumn(0, L"", LVCFMT_CENTER, 100);
	m_groupListCtrl.InsertColumn(1, L"", LVCFMT_CENTER, 100);

	cGroup = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


// CGroupDlg 消息处理程序

void CGroupDlg::OnClickGrouplist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMItemActivate->iItem > -1 && pNMItemActivate->iSubItem == 0)
	{
		m_vChatDlg[pNMItemActivate->iItem]->ShowWindow(SW_SHOW);
		m_groupListCtrl.SetItemText(pNMItemActivate->iItem, 1, CString(""));
	}
}


afx_msg LRESULT CGroupDlg::OnAddgroup(WPARAM wParam, LPARAM lParam)
{
	char *groupname = (char*)wParam;
	m_groupListCtrl.InsertItem(cGroup, L"");
	WCHAR buf[20] = { 0 };
	MultiByteToWideChar(CP_ACP, NULL, groupname, -1, buf, _countof(buf));
	m_groupListCtrl.SetItemText(cGroup, 0, buf);
	++cGroup;

	CChatDlg *pChatDlg = new CChatDlg;
	pChatDlg->m_dataType = GROUPMSG;

	pChatDlg->m_dataType = GROUPMSG;
	m_vChatDlg.push_back(pChatDlg);

	pChatDlg->m_friendname = groupname;
	pChatDlg->Create(IDD_CHATDLG);
	pChatDlg->SetWindowTextW(CString(groupname));
	pChatDlg->ShowWindow(SW_HIDE);
	pChatDlg->UpdateWindow();
	return 0;
}
