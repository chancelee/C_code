
// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "MainDlg.h"
#include "AddDlg.h"
#include "FriendDlg.h"
#include "GroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CChatDlg 对话框



CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAINDLG, pParent)
	, m_curChatValue(_T(""))
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMainDlg::OnSelchangeTab1)
	ON_BN_CLICKED(IDC_ADDBTN, &CMainDlg::OnBnClickedAddbtn)
END_MESSAGE_MAP()




// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	::SetWindowTextA(m_hWnd, g_client.m_nickname);

	m_tabCtrl.InsertItem(0, L"好友列表");
	m_tabCtrl.InsertItem(1, L"群聊");

	m_tabWnds[0] = new CFriendDlg;
	m_tabWnds[0]->Create(IDD_FRIENDDLG, &m_tabCtrl);
	m_tabWnds[1] = new CGroupDlg;
	m_tabWnds[1]->Create(IDD_GROUPDLG, &m_tabCtrl);

	CRect rect = { 0 };
	m_tabCtrl.GetClientRect(&rect);
	rect.DeflateRect(3, 25, 3, 3);
	for (int i = 0; i < 2; ++i)
	{
		m_tabWnds[i]->MoveWindow(&rect);
	}

	showTabWnd(0);

	CreateThread(NULL,
		NULL,
		chatRecvProc,
		this,
		0,
		NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMainDlg::showTabWnd(int index)
{
	for (int i = 0; i < 2; ++i)
	{
		m_tabWnds[i]->ShowWindow(i == index ? SW_SHOWNORMAL : SW_HIDE);
	}
}


/*
*	3 functions about friend messages.
*/

void CMainDlg::agreeFriend(DataPack * pDataPack)
{
	/*
	*	data format:
	*		ADDFRIEND,size
	*		fromname
	*/
	CStringA buf;
	CStringA data;

	buf.Format("%s请求添加为好友", pDataPack->data);
	int ret = MessageBoxA(NULL, buf, "好友请求", MB_OKCANCEL);
	if (ret == IDOK)
	{
		//MessageBoxA(NULL, "同意", "好友请求", MB_OKCANCEL);
		data.Format("%s:%d:%s", g_client.m_nickname, AGREE, pDataPack->data);
		g_client.send(ADDFRIEND, data);
	}
	else
	{
		//MessageBoxA(NULL, "拒绝", "好友请求", MB_OKCANCEL);
		data.Format("%d:%s:%s", DISAGREE, pDataPack->data, g_client.m_nickname);
		g_client.send(ADDFRIEND, data);
	}
}

void CMainDlg::addFriend(DataPack * pDataPack)
{
	/*
	*	pDataPack->data format:
	*		friendname
	*/
	CFriendDlg* pFriendDlg = (CFriendDlg*)m_tabWnds[0];
	pFriendDlg->addFriend(pDataPack->data);
}

void CMainDlg::recvFriendData(DataPack *pDataPack)
{
	CFriendDlg* pFriendDlg = (CFriendDlg*)m_tabWnds[0];
	pFriendDlg->recvFriendData(pDataPack);
	///*	
	//*	pDataPack->head
	//*		SENDTOFRIEND, size
	//*	pDataPack->data format:
	//*		fromname:data
	//*/
	//char *pFriendname = NULL, *pData = NULL;
	//pFriendname = strtok_s(pDataPack->data, ":", &pData);
	//if (!pFriendname || !pData)
	//{
	//	return;
	//}
	////int len = strlen(_friendname);
	//CStringA buf;
	//buf.Format("%s:\r\n  %s\r\n\r\n", pFriendname, pData);

	//CFriendDlg* pFriendDlg = (CFriendDlg*)m_tabWnds[0];
	//std::vector<CChatDlg*>::iterator it;

	//if (pFriendDlg->m_vChatDlg.size())
	//{
	//	for (it = pFriendDlg->m_vChatDlg.begin();
	//		(*it)->m_friendname != pFriendname 
	//			&& it != pFriendDlg->m_vChatDlg.end();
	//		++it);
	//	if (it != pFriendDlg->m_vChatDlg.end())
	//	{
	//		(*it)->m_chatEditCtrl.SetSel(-1);
	//		(*it)->m_chatEditCtrl.ReplaceSel(CString(buf));
	//	}
	//}

}


/*
*	3 functions about group messages.
*/

void CMainDlg::agreeJoinGroup(DataPack * pDataPack)
{
	/*
	*	data format:
	*		ADDFRIEND,size
	*		fromname:groupname
	*/

	CStringA buf;
	CStringA data;

	char *pFromname = NULL, *pGroupname = NULL;
	pFromname = strtok_s(pDataPack->data, ":", &pGroupname);
	if (!pFromname || !pGroupname)
	{
		return;
	}

	buf.Format("%s请求加入群聊(%s)", pFromname, pGroupname);
	int ret = MessageBoxA(NULL, buf, "好友请求", MB_OKCANCEL);
	if (ret == IDOK)
	{
		//MessageBoxA(NULL, "同意", "好友请求", MB_OKCANCEL);
		data.Format("%s:%d:%s", pFromname, AGREE, pGroupname);
		g_client.send(JOINGROUP, data);
	}
	else
	{
		//MessageBoxA(NULL, "拒绝", "好友请求", MB_OKCANCEL);
		data.Format("%d:%s:%s", DISAGREE, pDataPack->data, g_client.m_nickname);
		g_client.send(ADDFRIEND, data);
	}
}

//void CMainDlg::addGroup(DataPack * pDataPack)
//{
//	/*
//	*	pDataPack->data format:
//	*		groupname
//	*/
//	CGroupDlg* pGroupDlg = (CGroupDlg*)m_tabWnds[1];
//	pGroupDlg->addGroup(pDataPack->data);
//}

void CMainDlg::recvGroupData(DataPack *pDataPack)
{
	CGroupDlg* pGroupDlg = (CGroupDlg*)m_tabWnds[1];
	pGroupDlg->recvGroupData(pDataPack);
	
	/*
	std::vector<CChatDlg*>::iterator it;

	if (pGroupDlg->m_vChatDlg.size())
	{
		for (it = pGroupDlg->m_vChatDlg.begin();
			(*it)->m_friendname != pFriendname
			&& it != pGroupDlg->m_vChatDlg.end();
			++it);
		if (it != pGroupDlg->m_vChatDlg.end())
		{
			(*it)->m_chatEditCtrl.SetSel(-1);
			(*it)->m_chatEditCtrl.ReplaceSel(CString(buf));
		}
	}
	else
	{
		CChatDlg *pChatDlg = new CChatDlg;
		pGroupDlg->m_vChatDlg.push_back(pChatDlg);

		pChatDlg->Create(IDD_CHATDLG);
		pChatDlg->m_friendname.Format("%s", pFriendname);
		SetWindowTextA(pChatDlg->m_hWnd, pFriendname);
		pChatDlg->ShowWindow(SW_SHOWNORMAL);

		pChatDlg->m_chatEditCtrl.SetSel(-1);

		pChatDlg->m_chatEditCtrl.ReplaceSel(CString(buf));
	}
	*/
}


// CChatDlg 消息处理程序

void CMainDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	showTabWnd(m_tabCtrl.GetCurSel());
}

void CMainDlg::OnBnClickedAddbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddDlg addDlg;
	addDlg.DoModal();
}

DWORD WINAPI chatRecvProc(LPVOID arg)
{
	CMainDlg* pMainDlg = (CMainDlg*)arg;
	DataPack * pDataPack = NULL;

	while (1)
	{
		if (!g_client.recv(&pDataPack))
		{
			if (!pDataPack)
			{
				continue;
			}
			
			switch (pDataPack->dataPackHead.type)
			{
			case SENDTOFRIEND:
			{
				pMainDlg->recvFriendData(pDataPack);
				break;
			}
			case GROUPMSG:
			{
				pMainDlg->recvGroupData(pDataPack);
				break;
			}
			case ADDFRIEND:
			{
				pMainDlg->agreeFriend(pDataPack);
				break;
			}
			case SHOWFRIEND:
			{
				//pMainDlg->addFriend(pDataPack);
				SendMessage(pMainDlg->m_tabWnds[0]->m_hWnd, WM_ADDFRIEND, (WPARAM)pDataPack->data, 0);
				break;
			}
			case JOINGROUP:
			{
				pMainDlg->agreeJoinGroup(pDataPack);
				break;
			}
			case SHOWGROUP:
			{
				//pMainDlg->addGroup(pDataPack);
				SendMessage(pMainDlg->m_tabWnds[1]->m_hWnd, WM_ADDGROUP, (WPARAM)pDataPack->data,0);
				break;
			}
			free(pDataPack);
			pDataPack = NULL;
			}
		}
	}
}
