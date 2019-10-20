// FileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_2.h"
#include "FileDlg.h"
#include "afxdialogex.h"
#include "Tool.h"
#include "Md5.h"

// CFileDlg 对话框

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

#define	WM_FOUNDFILE	(WM_USER+100)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILEDLG, pParent)
	, m_strDir(_T(""))
	, m_strFilter(_T(""))
	, m_isScanning(FALSE)
{
	/*
	*	Update virus md5 database.
	*/
	//m_vVirusMd5.push_back("d41d8cd98f00b204e9800998ecf8427e");
}

CFileDlg::~CFileDlg()
{
	m_vFile.clear();
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEDIR, m_strDir);
	DDX_Text(pDX, IDC_FILTER, m_strFilter);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_SELECTALL, m_selectAll);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHOSEDIR, &CFileDlg::OnBnClickedScan)
	ON_MESSAGE(WM_FOUNDFILE, &CFileDlg::FOUNDFILE)
	ON_BN_CLICKED(IDC_SELECTALL, &CFileDlg::OnBnClickedSelectall)
	ON_BN_CLICKED(IDC_BTNDEL, &CFileDlg::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_VSCLEAR, &CFileDlg::OnBnClickedVsClear)
	ON_BN_CLICKED(IDC_BTNSCANALL, &CFileDlg::OnBnClickedBtnscanall)
END_MESSAGE_MAP()


// CFileDlg 消息处理程序


BOOL CFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listCtrl.InsertColumn(0, L"File Path", LVCFMT_CENTER, 240);
	m_listCtrl.InsertColumn(1, L"Size", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(2, L"Create", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(3, L"Modify", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(4, L"MD5", LVCFMT_CENTER, 100);


	m_strFilter = L".txt";
	m_vsFilter = L".ilk .pdb .obj .log .pch .tlog .lastbuildstate .sdf .idb .ipch .res .o .lst .knl .img .bin .db";
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CFileDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDropFiles(hDropInfo);

	TCHAR path[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, path, MAX_PATH);
	
	m_strDir = path;
	UpdateData(FALSE);

}

DWORD CALLBACK ScanDirProc(VOID *arg)
{
	CFileDlg *_this = (CFileDlg *)arg;

	findFiles(_this->m_strDir,
		_this->m_strFilter,
		&(_this->m_vFile));
	_this->MessageBox(L"扫描完毕");

	SendMessage(_this->m_hWnd, WM_FOUNDFILE, 0, 0);
	_this->m_isScanning = FALSE;

	return 0;
}

void CFileDlg::OnBnClickedScan()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_isScanning)
	{
		TerminateThread(m_hScanThread, 0);
		m_isScanning = false;
		return;
	}
	UpdateData(TRUE);


	// 如果目录为空,则弹出文件夹选择对话框
	if (m_strDir.IsEmpty())
	{
		TCHAR path[MAX_PATH] = { 0 };
		BROWSEINFO browsInfo = { 0 };
		browsInfo.pidlRoot = 0;
		browsInfo.lpszTitle = _T("Please choose a dir");
		browsInfo.ulFlags = BIF_DONTGOBELOWDOMAIN 
			| BIF_RETURNONLYFSDIRS 
			| BIF_NEWDIALOGSTYLE 
			| BIF_EDITBOX 
			| BIF_USENEWUI 
			| BIF_BROWSEINCLUDEFILES;
		browsInfo.lpfn = NULL;


		// 显示文件夹选择对话框
		LPITEMIDLIST lpItemList = ::SHBrowseForFolder(&browsInfo);
		if (lpItemList)
		{
			if (::SHGetPathFromIDList(lpItemList, path))
			{
				m_strDir = path;
				UpdateData(FALSE);
			}
			::CoTaskMemFree(lpItemList);
		}
		else
		{
			return;
		}

	}


	m_isScanning = TRUE;

	m_listCtrl.DeleteAllItems();
	m_vFile.clear();
	

	m_hScanThread = CreateThread(NULL, 0, ScanDirProc, this, 0,0);
}


afx_msg LRESULT CFileDlg::FOUNDFILE(WPARAM wParam, LPARAM lParam)
{
	if (!m_isScanning)
	{
		return -1;
	}

	//FILEINFO *fInfo = (FILEINFO*)wParam;
	//PWIN32_FIND_DATA pFindData = (PWIN32_FIND_DATA)wParam;
	PWIN32_FIND_DATA pFindData = NULL;

	CStringW strW;
	CStringA strA;
	SYSTEMTIME sysTime = { 0 };
	char szFilePath[MAX_PATH] = { 0 };
	char *md5 = NULL;

	size_t nFile = m_vFile.size();
	size_t nVirus = m_client.getMD5Num();
	BOOL bDelVirus = FALSE;
	

	for (size_t nRow = 0; nRow < nFile; ++nRow)
	{
		WideCharToMultiByte(CP_ACP, NULL, m_vFile[nRow]->cFileName, -1, szFilePath, _countof(szFilePath), NULL, FALSE);

		md5 = md5FileValue(szFilePath);

		/*
		*	Check virus.
		*		If yes, you can delete it;
		*/
		bDelVirus = FALSE;
		for (size_t i = 0; i < nVirus; ++i)
		{
			//if (!strcmp(md5, m_vVirusMd5[i]))
			if (!strcmp(md5, m_client.getMD5(i)))
			{
				strA.Format("Virus md5 matched:\n %s \n Delete it ?", szFilePath);
				strW = strA;
				if (MessageBoxW(strW, L"Warning!", MB_YESNO)
					== IDYES)
				{
					//DeleteFileA(szFilePath);
					strA.Format("%s", szFilePath);
					strW = strA;
					strW += L" deleted";
					MessageBoxW(strW);
					bDelVirus = TRUE;
					--nRow;
					break;
				}
			}
		}

		if (bDelVirus) continue;

		m_listCtrl.InsertItem(nRow, L"");

		/*
		*	File path
		*/
		strW.Format(L"%s", m_vFile[nRow]->cFileName);
		m_listCtrl.SetItemText(nRow, 0, strW);

		/*
		*	File size
		*/
		strW.Format(L"%dKB", (m_vFile[nRow]->nFileSizeHigh << 32 | m_vFile[nRow]->nFileSizeLow) / 1024);
		m_listCtrl.SetItemText(nRow, 1, strW);
		m_listCtrl.Scroll(CSize(0, 10000));

		/*
		*	Creating time
		*/
		FileTimeToSystemTime(&(m_vFile[nRow]->ftCreationTime), &sysTime);
		strW.Format(L"%04d-%02d-%02d-%02d:%02d:%02d", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		m_listCtrl.SetItemText(nRow, 2, strW);

		/*
		*	Last modified time
		*/
		FileTimeToSystemTime(&(m_vFile[nRow]->ftLastWriteTime), &sysTime);
		strW.Format(L"%04d-%02d-%02d-%02d:%02d:%02d",
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		m_listCtrl.SetItemText(nRow, 3, strW);

		
		/*
		*	md5
		*/
		strA.Format("%s", md5);
		strW = strA;
		m_listCtrl.SetItemText(nRow, 4, strW);
		

	}

	return 0;
}



void CFileDlg::OnBnClickedSelectall()
{
	int n = m_listCtrl.GetItemCount();
	for (int i = 0; i < n; ++i) {
		m_listCtrl.SetCheck(i, !m_listCtrl.GetCheck(i));
	}
}


void CFileDlg::OnBnClickedBtnDel()
{
	int n = m_listCtrl.GetItemCount();
	CString path;
	for (int i = 0; i < n; ++i) 
	{
		if (m_listCtrl.GetCheck(i))
		{
			path = m_listCtrl.GetItemText(i, 0);
			OutputDebugString(L"delete file " + path + L"\n");
			//DeleteFileW(path);
			m_listCtrl.DeleteItem(i);
		}
	}
}


void CFileDlg::OnBnClickedVsClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strFilter = m_vsFilter;
	UpdateData(FALSE);
}


void CFileDlg::OnBnClickedBtnscanall()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if (m_isScanning)
	{
		TerminateThread(m_hScanThread, 0);
		m_isScanning = false;
		return;
	}
	UpdateData(TRUE);


	// 如果目录为空,则弹出文件夹选择对话框
	if (m_strDir.IsEmpty())
	{
		TCHAR path[MAX_PATH] = { 0 };
		BROWSEINFO browsInfo = { 0 };
		browsInfo.pidlRoot = 0;
		browsInfo.lpszTitle = _T("Please choose a dir");
		browsInfo.ulFlags = BIF_DONTGOBELOWDOMAIN
			| BIF_RETURNONLYFSDIRS
			| BIF_NEWDIALOGSTYLE
			| BIF_EDITBOX
			| BIF_USENEWUI
			| BIF_BROWSEINCLUDEFILES;
		browsInfo.lpfn = NULL;


		// 显示文件夹选择对话框
		LPITEMIDLIST lpItemList = ::SHBrowseForFolder(&browsInfo);
		if (lpItemList)
		{
			if (::SHGetPathFromIDList(lpItemList, path))
			{
				m_strDir = path;
				UpdateData(FALSE);
			}
			::CoTaskMemFree(lpItemList);
		}
		else
		{
			return;
		}

	}


	m_isScanning = TRUE;

	m_listCtrl.DeleteAllItems();
	m_vFile.clear();

	m_strFilter.Empty();
	m_hScanThread = CreateThread(NULL, 0, ScanDirProc, this, 0, 0);
}
