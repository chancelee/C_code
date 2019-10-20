
// TaskManager_2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskManager_2.h"
#include "TaskManager_2Dlg.h"
#include "afxdialogex.h"

#include "ProcessDlg.h"
#include "HwDlg.h"
#include "WndDlg.h"
#include "FileDlg.h"
#include "PE.h"
#include "Service.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//#define WM_FLUSHCPU		(WM_USER+100)

enum timeId { TIMEID_1 = 0x1000};

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CTaskManager_2Dlg �Ի���



CTaskManager_2Dlg::CTaskManager_2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TASKMANAGER_2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTaskManager_2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CTaskManager_2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTaskManager_2Dlg::OnTcnSelchangeTab1)
	//ON_MESSAGE(WM_FLUSHCPU, &CTaskManager_2Dlg::OnFlushcpu)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CTaskManager_2Dlg::flushStatus()
{
	MYMEMINFO meminfo;
	CString buf;

	GetMemInfo(meminfo);

	buf.Format(L"CPU occupancy: %.2f%%", meminfo.cpuOccu);
	m_status.SetPaneText(0, buf);

	/*buf.Format(L"%uMB", (meminfo.stcMemStatus.dwTotalPhys - meminfo.stcMemStatus.dwAvailPhys) / 1048576);*/
	buf.Format(L"Free mem: %uMB", meminfo.stcMemStatus.dwAvailPhys / 1048576);
	m_status.SetPaneText(1, buf);
}


// CTaskManager_2Dlg ��Ϣ�������
BOOL EnableDebugPrivilege(BOOL fEnable) {   //����Ϊ����Ȩ��
	BOOL fOk = FALSE;    HANDLE hToken;
	// ���޸�Ȩ�޵ķ�ʽ���򿪽��̵�����
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES,
		&hToken)) {
		// ����Ȩ�޽ṹ��
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		//���LUID
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
		// ����Ȩ��
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL); //�޸�Ȩ��
		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return(fOk);
}
BOOL CTaskManager_2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	EnableDebugPrivilege(TRUE);

	int cTab = _countof(m_tabs);

	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);


	m_tabCtrl.InsertItem(1, L"Process");
	m_tabCtrl.InsertItem(2, L"Windows");
	m_tabCtrl.InsertItem(3, L"Hardware");
	m_tabCtrl.InsertItem(4, L"File");
	m_tabCtrl.InsertItem(5, L"PE Analyse");
	m_tabCtrl.InsertItem(6, L"Service");

	m_tabs[0] = new CProcessDlg;
	m_tabs[0]->Create(IDD_PROCESSDLG, &m_tabCtrl);
	m_tabs[1] = new CWndDlg;
	m_tabs[1]->Create(IDD_WNDDLG, &m_tabCtrl);
	m_tabs[2] = new CHwDlg;
	m_tabs[2]->Create(IDD_HWDLG, &m_tabCtrl);
	m_tabs[3] = new CFileDlg;
	m_tabs[3]->Create(IDD_FILEDLG, &m_tabCtrl);
	m_tabs[4] = new CPE;
	m_tabs[4]->Create(IDD_PE, &m_tabCtrl);
	m_tabs[5] = new CService;
	m_tabs[5]->Create(IDD_SERVICE, &m_tabCtrl);

	CRect rect = { 0};
	m_tabCtrl.GetClientRect(&rect);
	rect.DeflateRect(0, 30, 0, 10);
	for (int i = 0; i < cTab; ++i)
	{
		m_tabs[i]->MoveWindow(&rect);
	}
	ShowTabWnd(0);

	
	/*
	*	Status bar.
	*/
	UINT aId[] = {0x101, 0x102, 0x103};
	
	m_status.Create(this);
	m_status.SetIndicators(aId, _countof(aId));
	m_status.SetPaneInfo(0, aId[0], SBPS_STRETCH, 100);
	m_status.SetPaneInfo(1, aId[1], SBPS_STRETCH, 100);
	m_status.SetPaneInfo(2, aId[2], SBPS_STRETCH, 100);
	m_status.SetPaneText(0, L"");
	m_status.SetPaneText(1, L"");
	m_status.SetPaneText(2, L"");

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	
	flushStatus();
	SetTimer(TIMEID_1, 10000, 0);


	/*
	*	Hot key
	*	Elevate first.
	*/
	m_bIsWindowHide = FALSE;
	::RegisterHotKey(this->GetSafeHwnd(),
		0xa001,
		MOD_ALT,
		'K');

	//SetWindowTextW(L"Safety-guard 2019-10 by starr");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTaskManager_2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTaskManager_2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTaskManager_2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTaskManager_2Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	ShowTabWnd(m_tabCtrl.GetCurSel());
}

void CTaskManager_2Dlg::ShowTabWnd(int index)
{
	int num = _countof(m_tabs);
	for (int i = 0; i < num; ++i)
	{
		m_tabs[i]->ShowWindow(i == index ? SW_SHOW : SW_HIDE);
	}
}



void CTaskManager_2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMEID_1)
	{
		flushStatus();
	}
	CDialogEx::OnTimer(nIDEvent);
}



BOOL CTaskManager_2Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->message == WM_HOTKEY)
		&& (pMsg->wParam == 0xa001))
	{
		if (m_bIsWindowHide)
		{
			ShowWindow(SW_NORMAL);
			m_bIsWindowHide = FALSE;
		}
		else
		{
			ShowWindow(SW_HIDE);
			m_bIsWindowHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
