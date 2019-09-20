// LogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Chat.h"
#include "LogDlg.h"
#include "afxdialogex.h"




// CLogDlg 对话框

IMPLEMENT_DYNAMIC(CLogDlg, CDialogEx)

CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGDLG, pParent)
	, m_nickNameValue(_T(""))
	, m_passwordValue(_T(""))
{

}

CLogDlg::~CLogDlg()
{
}

void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_nickNameValue);
	DDX_Text(pDX, IDC_EDIT1, m_passwordValue);
	DDX_Control(pDX, IDC_REGBTN, m_regCtrl);
	DDX_Control(pDX, IDC_LOGBTN, m_passwordCtrl);
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REGBTN, &CLogDlg::OnBnClickedRegbtn)
	ON_BN_CLICKED(IDC_LOGBTN, &CLogDlg::OnBnClickedLogbtn)
END_MESSAGE_MAP()


// CLogDlg 消息处理程序


void CLogDlg::OnBnClickedRegbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	char nickname[20] = { 0 };
	char password[20] = { 0 };

	UpdateData(TRUE);

	WideCharToMultiByte(CP_ACP, NULL, m_nickNameValue, -1, nickname, _countof(nickname), NULL, FALSE);
	WideCharToMultiByte(CP_ACP, NULL, m_passwordValue, -1, password, _countof(password), NULL, FALSE);
	g_client.regist(nickname, password);



}


BOOL CLogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowTextW(L"登录");

	CreateThread(NULL,
		NULL,
		logRecvProc,
		this,
		0,
		NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLogDlg::OnBnClickedLogbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	char nickname[20] = { 0 };
	char password[20] = { 0 };

	UpdateData(TRUE);
	/*MessageBoxW(m_nickNameValue);
	MessageBoxW(m_passwordValue);*/
	WideCharToMultiByte(CP_ACP, NULL, m_nickNameValue, -1, nickname, _countof(nickname), NULL, FALSE);
	WideCharToMultiByte(CP_ACP, NULL, m_passwordValue, -1, password, _countof(password), NULL, FALSE);
	g_client.login(nickname, password);
}


DWORD WINAPI logRecvProc(LPVOID arg)
{
	CLogDlg* pLogDlg = (CLogDlg*)arg;
	DataPack * pDataPack = NULL;

	while (1)
	{
		if (!g_client.recv(&pDataPack))
		{
			if (!pDataPack)
			{
				continue;
			}
			/*printf("recv from server:\n   %s\n", pDataPack->data);
			printf("\ninput:");*/
			switch (pDataPack->dataPackHead.type)
			{
			case REGISTER:
				if (atoi(pDataPack->data))
				{
					MessageBoxW(NULL, L"注册成功", L"", 0);
				}
				else
				{
					MessageBoxW(NULL, L"注册失败", L"", 0);
					//exit(0);
				}
				break;
			case LOGIN:
				if (atoi(pDataPack->data))
				{
					MessageBoxW(NULL, L"登录成功", L"", 0);
					pLogDlg->ShowWindow(SW_HIDE);
					pLogDlg->EndDialog(IDOK);
					_endthreadex(0);
				}
				else
				{
					MessageBoxW(NULL, L"登录失败", L"", 0);
					//exit(0);
				}
			}
			free(pDataPack);
			pDataPack = NULL;
		}
	}
}
