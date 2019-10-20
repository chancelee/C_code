// Service.cpp : 实现文件
//
#include "stdafx.h"


#include "TaskManager_2.h"
#include "Service.h"
#include "afxdialogex.h"

// CService 对话框

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVICE, pParent)
	, m_pEnumService(NULL)
{
	m_hScManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!m_hScManager)
	{
		MessageBoxW(L"OpenSCManager() error\n");
	}
}

CService::~CService()
{

}

void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CService, CDialogEx)
	ON_COMMAND(ID_SERVICE_START, &CService::OnServiceStart)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CService::OnRclickList1)
	ON_COMMAND(ID_SERVICE_FLUSH, &CService::OnServiceFlush)
	ON_COMMAND(ID_SERVICE_STOP, &CService::OnServiceStop)
END_MESSAGE_MAP()



void CService::enumServices()
{
	SC_HANDLE hService = NULL;
	LPQUERY_SERVICE_CONFIG pServiceConfig = NULL;
	DWORD dwBufSize = 0, dwNumService = 0;

	DWORD dwServiceTypes[] = {
		SERVICE_FILE_SYSTEM_DRIVER,
		SERVICE_KERNEL_DRIVER,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_WIN32_SHARE_PROCESS
	};
	wchar_t *szServiceTypes[] = {
		L"File system driver.",
		L"Device driver.",
		L"Runs in its own process.",
		L"Shares a process with other services."
	};
	wchar_t *szServiceStates[] = {
		L"",
		L"Stopped.",
		L"Starting.",
		L"Stopping.",
		L"Running.",
		L"About to ontinue.",
		L"Pausing.",
		L"Paused."
	};
	wchar_t *szStartTypes[] = {
		L"SERVICE_BOOT_START  ",
		L"SERVICE_SYSTEM_START",
		L"SERVICE_AUTO_START  ",
		L"SERVICE_DEMAND_START",
		L"SERVICE_DISABLED    "
	};

	CStringA strA;
	CStringW strW;
	unsigned int nRow = 0;

	if (m_pEnumService)
	{
		free(m_pEnumService);
		m_pEnumService = NULL;
	}



	/*
	*	Get the bufsize.
	*/
	EnumServicesStatusEx(m_hScManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		NULL, 0,
		&dwBufSize, &dwNumService,
		NULL, NULL);

	m_pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)(calloc(1, dwBufSize));

	/*
	*	Get the service structs.
	*/
	if (!EnumServicesStatusEx(m_hScManager, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		(LPBYTE)m_pEnumService, dwBufSize,
		&dwBufSize, &dwNumService,
		NULL, NULL))
	{
		if (m_pEnumService)
		{
			free(m_pEnumService);
			m_pEnumService = NULL;
		}
		MessageBoxW(L"EnumServicesStatusEx() error\n");
		return;
	}

	for (DWORD i = 0; i < dwNumService; ++i, ++nRow)
	{
		
		m_listCtrl.InsertItem(nRow, L"");
		m_listCtrl.SetItemText(nRow, 0, m_pEnumService[i].lpServiceName);


		//wprintf(L"service type:\n");
		for (DWORD j = 0; j < _countof(dwServiceTypes); ++j)
		{
			if (m_pEnumService[i].ServiceStatusProcess.dwServiceType == dwServiceTypes[j])
			{
				//wprintf(L"\t%s\n", szServiceTypes[j]);
				m_listCtrl.SetItemText(nRow, 1, szServiceTypes[j]);

			}
		}

		//wprintf(L"service state:\n");
		for (DWORD j = 1; j < 8; ++j)
		{
			if (j == m_pEnumService[i].ServiceStatusProcess.dwCurrentState)
			{
				m_listCtrl.SetItemText(nRow, 2, szServiceStates[j]);

			}
		}

		hService = OpenService(m_hScManager, m_pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		if (!hService)
		{
			break;
		}

		QueryServiceConfig(hService, NULL, 0, &dwBufSize);
		pServiceConfig = (LPQUERY_SERVICE_CONFIG)calloc(1, dwBufSize);
		QueryServiceConfig(hService, pServiceConfig, dwBufSize, &dwBufSize);

		//wprintf(L"Start type:\n");
		for (DWORD j = 0; j < _countof(szStartTypes); ++j)
		{
			if (j == pServiceConfig->dwStartType)
			{
				//wprintf(L"\t%s\n", szStartTypes[j]);
				m_listCtrl.SetItemText(nRow, 3, szStartTypes[j]);
			}
		}
		//wprintf(L"BinaryPathName:	%s\n", pServiceConfig->lpBinaryPathName);
		m_listCtrl.SetItemText(nRow, 4, pServiceConfig->lpBinaryPathName);

		if (pServiceConfig)
		{
			free(pServiceConfig);
			pServiceConfig = NULL;
		}
	}


	if (pServiceConfig)
	{
		free(pServiceConfig);
		pServiceConfig = NULL;
	}
}

// CService 消息处理程序


BOOL CService::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwOldStyle = m_listCtrl.GetExtendedStyle();
	m_listCtrl.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_CHECKBOXES);

	m_listCtrl.InsertColumn(0, L"Service name", LVCFMT_CENTER, 150);
	m_listCtrl.InsertColumn(1, L"Service type", LVCFMT_CENTER, 200);
	m_listCtrl.InsertColumn(2, L"Service state", LVCFMT_CENTER, 150);
	m_listCtrl.InsertColumn(3, L"Start type", LVCFMT_CENTER, 350);
	m_listCtrl.InsertColumn(4, L"BinPath", LVCFMT_CENTER, 350);

	enumServices();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CService::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CMenu *pMenu = new CMenu;
	pMenu->LoadMenuW(IDR_PROCESS);
	CMenu *pSubMenu = pMenu->GetSubMenu(2);

	CPoint point = { 0 };
	GetCursorPos(&point);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);
	*pResult = 0;
}


void CService::OnServiceStart()
{
	// TODO: 在此添加命令处理程序代码

	SC_HANDLE hService = NULL;

	int index = (int)m_listCtrl.GetFirstSelectedItemPosition();
	if (index == 0) return;

	--index;


	hService = OpenService(m_hScManager, m_pEnumService[index].lpServiceName, SERVICE_START);
	if (!hService)
	{
		MessageBoxW(L"OpenService() error", L"error", 0);
	}

	StartServiceW(hService, NULL, NULL);
}



void CService::OnServiceStop()
{
	// TODO: 在此添加命令处理程序代码
	SC_HANDLE hService = NULL;
	SERVICE_STATUS stcService = { 0 };

	int index = (int)m_listCtrl.GetFirstSelectedItemPosition();
	if (index == 0) return;

	--index;


	hService = OpenService(m_hScManager, m_pEnumService[index].lpServiceName, SERVICE_STOP);
	if (!hService)
	{
		MessageBoxW(L"OpenService() error", L"error", 0);
	}

	if (ControlService(hService, SERVICE_CONTROL_STOP, &stcService))
	{
		m_listCtrl.DeleteItem(index);
	}
	else
	{
		MessageBoxW(L"ControlService() error", L"error", 0);
	}
}

void CService::OnServiceFlush()
{
	// TODO: 在此添加命令处理程序代码
	enumServices();
}

