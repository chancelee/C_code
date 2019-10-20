// ProcessSubDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_2.h"
#include "ProcessSubDialog.h"
#include "afxdialogex.h"


// ProcessSubDialog 对话框

IMPLEMENT_DYNAMIC(ProcessSubDialog, CDialogEx)

ProcessSubDialog::ProcessSubDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSSUBDIALOG, pParent)
{

}

ProcessSubDialog::ProcessSubDialog(int type, CString strCap,DWORD pid, CWnd * pParent)
	:m_strCap(strCap),
	m_type(type),
	m_pid(pid)
{}

ProcessSubDialog::~ProcessSubDialog()
{
}

void ProcessSubDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESSSUBLIST, m_processSubListCtrl);
}




BEGIN_MESSAGE_MAP(ProcessSubDialog, CDialogEx)
END_MESSAGE_MAP()


void ProcessSubDialog::AddModules()
{
	if (!EnumModules96(m_pid, m_vModules))
		return;

	size_t cnt = m_vModules.size();
	CString buf;
	for (int i = 0; i < cnt; ++i)
	{
		m_processSubListCtrl.InsertItem(i, L"");

		m_processSubListCtrl.SetItemText(i, 0, m_vModules[i].wszFileName);
		buf.Format(L"%p", m_vModules[i].stcModinfo.EntryPoint);
		m_processSubListCtrl.SetItemText(i, 1, buf);
		buf.Format(L"%p", m_vModules[i].stcModinfo.lpBaseOfDll);
		m_processSubListCtrl.SetItemText(i, 2, buf);
		buf.Format(L"%u", m_vModules[i].stcModinfo.SizeOfImage);
		m_processSubListCtrl.SetItemText(i, 3, buf);

	}
}

void ProcessSubDialog::AddThreads()
{
	if (!EnumThreads(m_pid, m_vThreads))
		return;

	int cnt = m_vThreads.size();
	CString buf;
	for (int i = 0; i < cnt; ++i)
	{
		m_processSubListCtrl.InsertItem(i, L"");

		buf.Format(L"%d", m_vThreads[i].th32ThreadID);
		m_processSubListCtrl.SetItemText(i, 0, buf);
		buf.Format(L"%d", m_vThreads[i].tpBasePri);
		m_processSubListCtrl.SetItemText(i, 1, buf);
	}
}

// ProcessSubDialog 消息处理程序

BOOL ProcessSubDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwOldStyle = m_processSubListCtrl.GetExtendedStyle();

	SetWindowText(m_strCap);

	m_processSubListCtrl.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES);

	if (THREAD == m_type)
	{
		m_processSubListCtrl.InsertColumn(0, L"TID", LVCFMT_CENTER, 200);
		m_processSubListCtrl.InsertColumn(1,L"BasePri", LVCFMT_CENTER,200);
		AddThreads();
	}
	else if (MODULE == m_type)
	{
		m_processSubListCtrl.InsertColumn(0, L"Module name", LVCFMT_CENTER, 200);
		m_processSubListCtrl.InsertColumn(1, L"EntryPoint", LVCFMT_CENTER, 200);
		m_processSubListCtrl.InsertColumn(2, L"lpBaseOfDll", LVCFMT_CENTER, 200);
		m_processSubListCtrl.InsertColumn(3, L"SizeOfImage", LVCFMT_CENTER, 200);
		AddModules();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


