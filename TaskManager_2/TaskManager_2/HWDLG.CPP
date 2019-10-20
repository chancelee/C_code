// HWDlg.cpp : 实现文件
//

#include <vector>
#include "stdafx.h"
#include "TaskManager_2.h"
#include "HwDlg.h"
#include "afxdialogex.h"
#include "Tool.h"

// CHWDlg 对话框

IMPLEMENT_DYNAMIC(CHwDlg, CDialogEx)

CHwDlg::CHwDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HWDLG, pParent)
	, m_cProcValue(_T(""))
	, m_pageSizeValue(_T(""))
	, m_cpuTypeValue(_T(""))
	, m_phyMemValue(_T(""))
	, m_virtValue(_T(""))
{

}

CHwDlg::~CHwDlg()
{
}

void CHwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CPROC, m_cProcValue);
	DDX_Text(pDX, IDC_NPAGE, m_pageSizeValue);
	DDX_Text(pDX, IDC_CPUTYPE, m_cpuTypeValue);
	DDX_Text(pDX, IDC_PHYMEM, m_phyMemValue);
	DDX_Text(pDX, IDC_VIRTMEM, m_virtValue);
}


BEGIN_MESSAGE_MAP(CHwDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNOPTIMIZE, &CHwDlg::OnBnClickedBtnoptimize)
END_MESSAGE_MAP()


// CHWDlg 消息处理程序
BOOL CHwDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SYSTEM_INFO systemInfo;
	MEMORYSTATUS memStatus;

	GetSystemInfo(&systemInfo);
	GlobalMemoryStatus(&memStatus);

	m_cProcValue.Format(L"%d", systemInfo.dwNumberOfProcessors);
	m_pageSizeValue.Format(L"%d KB", systemInfo.dwPageSize / 1024);
	m_cpuTypeValue.Format(L"%d", systemInfo.dwProcessorType);

	m_phyMemValue.Format(L"%d MB", memStatus.dwTotalPhys / 1024 / 1024 );
	m_virtValue.Format(L"%d MB", memStatus.dwTotalVirtual / 1024 / 1024 );

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CHwDlg::OnBnClickedBtnoptimize()
{
	// TODO: 在此添加控件通知处理程序代码
	MEMORYSTATUSEX stcMemStatus = { 0 };
	DWORDLONG preOptimUsedMem = 0, afterOptimUsedMem = 0;
	std::vector<PROCESSINFO> vProcess;
	DWORD dwNumProcess = 0;
	HANDLE hProcess = NULL;
	CString buf;

	stcMemStatus.dwLength = sizeof(stcMemStatus);

	GetProcesses(vProcess);
	dwNumProcess = vProcess.size();

	GlobalMemoryStatusEx(&stcMemStatus);
	preOptimUsedMem = stcMemStatus.ullTotalPhys - stcMemStatus.ullAvailPhys;

	for (DWORD i = 0; i < dwNumProcess; ++i)
	{
		hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE, vProcess[i].th32ProcessID);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}

	GlobalMemoryStatusEx(&stcMemStatus);
	afterOptimUsedMem = stcMemStatus.ullTotalPhys - stcMemStatus.ullAvailPhys;

	buf.Format(L"Mem used before optimization: %d MB\n"
		L"Mem used after optimization: %d MB",
		preOptimUsedMem / 1024 / 1024,
		afterOptimUsedMem / 1024 / 1024);
	MessageBoxW(buf);
}
