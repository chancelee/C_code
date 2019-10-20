// WndDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_2.h"
#include "WndDlg.h"
#include "afxdialogex.h"


// CWndDlg 对话框

IMPLEMENT_DYNAMIC(CWndDlg, CDialogEx)

CWndDlg::CWndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WNDDLG, pParent)
{

}

CWndDlg::~CWndDlg()
{
}

void CWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WNDLIST, m_wndListCtrl);
}



BEGIN_MESSAGE_MAP(CWndDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_WNDLIST, &CWndDlg::OnRclickWndlist)
	ON_COMMAND(ID_FLUSH_FLUSH, &CWndDlg::OnFlushFlush)
END_MESSAGE_MAP()

void CWndDlg::updateWindowList()
{
	m_wndListCtrl.DeleteAllItems();
	EnumWindows(EnumWindowProc, (LPARAM)&m_wndListCtrl);
}

// CWndDlg 消息处理程序


BOOL CWndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwOldStyle = m_wndListCtrl.GetExtendedStyle();
	m_wndListCtrl.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES);
	m_wndListCtrl.InsertColumn(0, L"Window Name", LVCFMT_CENTER, 200);
	m_wndListCtrl.InsertColumn(0, L"Handle", LVCFMT_CENTER, 200);

	updateWindowList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}





void CWndDlg::OnRclickWndlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu *pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_PROCESS);
	CMenu *pSubMenu = pMenu->GetSubMenu(1);

	CPoint point = { 0 };
	GetCursorPos(&point);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);
}


void CWndDlg::OnFlushFlush()
{
	// TODO: 在此添加命令处理程序代码
	updateWindowList();
}
