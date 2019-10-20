// ProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_2.h"
#include "afxdialogex.h"
#include "ProcessDlg.h"
#include "ProcessSubDialog.h"

// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSDLG, pParent)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESSSUBLIST, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_PROCESSSUBLIST, &CProcessDlg::OnNMRClickList1)
	ON_COMMAND(ID_PROCESS_FLUSH, &CProcessDlg::OnProcessFlush)
	ON_COMMAND(ID_PROCESS_KILLPROCESS, &CProcessDlg::OnProcessKillprocess)
	ON_COMMAND(ID_PROCESS_THREAD, &CProcessDlg::OnProcessThread)
	ON_COMMAND(ID_PROCESS_MODULE, &CProcessDlg::OnProcessModule)
	ON_WM_SIZE()
	ON_COMMAND(ID_PROCESS_PROTECTALL, &CProcessDlg::OnProcessProtectall)
END_MESSAGE_MAP()



BOOL CProcessDlg::UpdataProcessList()
{
	int cCurItemCount = 0;
	if (cCurItemCount = m_vProcess.size())
	{

		m_vProcess.clear();
	}

	if (!GetProcesses(m_vProcess))
	{
		return FALSE;
	}


	int cNewItemCount = m_vProcess.size();

	for (int i = cCurItemCount; i < cNewItemCount; ++i)
	{
		m_listCtrl.InsertItem(i, L"");
	}
	for (int i = cNewItemCount; i < cCurItemCount; ++i)
	{
		m_listCtrl.SetItemText(i, 0, L"");
		m_listCtrl.SetItemText(i, 1, L"");
		m_listCtrl.SetItemText(i, 2, L"");
		m_listCtrl.SetItemText(i, 3, L"");
		m_listCtrl.SetItemText(i, 4, L"");
		m_listCtrl.SetItemText(i, 5, L"");
	}

	CString buf;
	DWORD dwPid = 0;
	for (int i = 0; i < cNewItemCount; ++i)
	{
		m_listCtrl.SetItemText(i, 0, m_vProcess[i].szExeFile);

		buf.Format(L"%d", m_vProcess[i].th32ProcessID);
		m_listCtrl.SetItemText(i, 1, buf);

		buf.Format(L"%d", m_vProcess[i].th32ParentProcessID);
		m_listCtrl.SetItemText(i, 2, buf);

		buf.Format(L"%d", m_vProcess[i].cntThreads);
		m_listCtrl.SetItemText(i, 3, buf);

		/*buf.Format(pi->dwMemoryUsage < 1024 ?
		L"%dB" : pi->dwMemoryUsage < 1048576 ?
		L"%dKB" : L"%dMB",
		pi->dwMemoryUsage < 1024 ?
		pi->dwMemoryUsage : pi->dwMemoryUsage < 1048576 ?
		pi->dwMemoryUsage / 1024 : pi->dwMemoryUsage / 1048576);
		m_listCtrl.SetItemText(index, 4, buf);

		buf.Format(L"%4.2f%%", pi->dCpuUsage);
		m_listCtrl.SetItemText(index, 5, buf);*/

		DWORD dMemUsage = GetProcessMemoryUsage(m_vProcess[i].th32ProcessID);
		buf.Format(dMemUsage < 1024 ?
			L"%dB" : dMemUsage < 1048576 ?
			L"%dKB" : L"%dMB",
			dMemUsage < 1024 ?
			dMemUsage : dMemUsage < 1048576 ?
			dMemUsage / 1024 : dMemUsage / 1048576);
		m_listCtrl.SetItemText(i, 4, buf);

		double cpuUsage = GetProcessCpuUsage(m_vProcess[i].th32ProcessID);
		if (abs(m_vProcess[i].dCpuUsage - cpuUsage) >= 0.001)
		{
			buf.Format(L"%4.2f%%", cpuUsage);
			m_listCtrl.SetItemText(i, 5, buf);
			m_vProcess[i].dCpuUsage = cpuUsage;
		}

	}

	return TRUE;
}


// CProcessDlg 消息处理程序


BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwOldStyle = m_listCtrl.GetExtendedStyle();

	m_listCtrl.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES);

	m_listCtrl.InsertColumn(0, L"Process name", LVCFMT_CENTER, 200);
	m_listCtrl.InsertColumn(1, L"PID", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(2, L"PPID", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(3, L"Thread count", LVCFMT_CENTER, 200);
	m_listCtrl.InsertColumn(4, L"Memory", LVCFMT_CENTER, 200);
	m_listCtrl.InsertColumn(5, L"CPU usage", LVCFMT_CENTER, 100);

	UpdataProcessList();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_listCtrl.m_hWnd)
	{
		m_listCtrl.SetWindowPos(0,0,0,cx,cy,SWP_NOZORDER);
	}
}

void CProcessDlg::OnProcessFlush()
{
	// TODO: 在此添加命令处理程序代码
	UpdataProcessList();
}


void CProcessDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu *pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_PROCESS);
	CMenu *pSubMenu = pMenu->GetSubMenu(0);

	CPoint point = { 0 };
	GetCursorPos(&point);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);

}

void CProcessDlg::OnProcessThread()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_listCtrl.GetFirstSelectedItemPosition();
	if (index == 0) return;

	--index;
	CString strCap;
	strCap.Format(L"%s Threads", m_listCtrl.GetItemText(index, 0));

	ProcessSubDialog *pThreadDialog = new ProcessSubDialog(
		THREAD, 
		strCap,
		m_vProcess[index].th32ProcessID);
	pThreadDialog->Create(IDD_PROCESSSUBDIALOG);
	pThreadDialog->ShowWindow(SW_NORMAL);
	
}


void CProcessDlg::OnProcessModule()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_listCtrl.GetFirstSelectedItemPosition();
	if (index == 0) return;

	--index;
	CString strCap;
	strCap.Format(L"%s Modules", m_listCtrl.GetItemText(index, 0));

	ProcessSubDialog *pModuleDialog = new ProcessSubDialog(
		MODULE,
		strCap,
		m_vProcess[index].th32ProcessID);
	pModuleDialog->Create(IDD_PROCESSSUBDIALOG);
	pModuleDialog->ShowWindow(SW_NORMAL);
}

void CProcessDlg::OnProcessKillprocess()
{
	// TODO: 在此添加命令处理程序代码
	int index = (int)m_listCtrl.GetFirstSelectedItemPosition();
	if (index == 0) return;
	
	--index;
	/*CString strPID = m_listCtrl.GetItemText(index, 1);*/
	DWORD dwPid = m_vProcess[index].th32ProcessID;

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	if (hProcess == 0)
	{
		CString error;
		error.Format(L"OpenProcess() error: %d", GetLastError());
		MessageBoxW(error);
		return;
	}

	TerminateProcess(hProcess, 0);

	m_listCtrl.DeleteItem(index);
	std::vector<PROCESSINFO>::iterator it = m_vProcess.begin() + index;
	m_vProcess.erase(it);
}




void CProcessDlg::OnProcessProtectall()
{
	// TODO: 在此添加命令处理程序代码
	size_t nProcess = m_vProcess.size();
	HMODULE hDll = NULL;
	DWORD dwPID = 0;
	HANDLE hProcess = NULL;
	LPVOID lpDllPath = NULL;
	char szDllPath[] = "F:\\proooooooooooogram\\15pb\\TaskManager_2\\x64\\Debug\\hook.dll";
	SIZE_T dwSize = 0;
	HANDLE hThread = NULL;

	/*
	*	CreateMapping to send protected PID to dll.
	*/
	HANDLE hMap = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		16,
		L"PID");
	if (NULL == hMap || INVALID_HANDLE_VALUE == hMap)
	{
		return;
	}
	LPVOID lpBuf = MapViewOfFile(
		hMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(DWORD));
	if (!lpBuf) return;

	dwPID = GetCurrentProcessId();
	memcpy(lpBuf, &dwPID, sizeof(dwPID));

	for (size_t i = 0; i < nProcess; ++i)
	{
		if (!wcscmp(m_vProcess[i].szExeFile,
			L"Taskmgr.exe"))
		{
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_vProcess[i].th32ProcessID);
			if (!hProcess) break;

			lpDllPath = VirtualAllocEx(hProcess,
				NULL,
				strlen(szDllPath) + 1,
				MEM_RESERVE | MEM_COMMIT,
				PAGE_EXECUTE_READWRITE);
			WriteProcessMemory(hProcess,
				lpDllPath,
				szDllPath,
				strlen(szDllPath) + 1,
				&dwSize);
			hThread = CreateRemoteThread(hProcess, NULL, 0,
				(LPTHREAD_START_ROUTINE)LoadLibraryA,
				lpDllPath,
				0, NULL);
			WaitForSingleObject(hThread, -1);

			OpenProcess(PROCESS_ALL_ACCESS, FALSE, 1);
			VirtualFreeEx(hProcess, lpDllPath, 0, MEM_RELEASE);

		}
	}
	CloseHandle(hThread);
}
