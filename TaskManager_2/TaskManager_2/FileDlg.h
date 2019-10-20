#pragma once

#include <vector>
#include "afxcmn.h"
#include "afxwin.h"

#include "Client.h"

/*
*	Functions of this class:
*		Scan and display file information with a filter;
*		Scan all files without filter;
*		Check virus md5 while scanning.
*/

// CFileDlg �Ի���

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	Client m_client;

	CString m_strDir;
	CString m_strFilter;
	CString m_vsFilter;
	CListCtrl m_listCtrl;

	std::vector<PWIN32_FIND_DATA> m_vFile;
	//std::vector<char*> m_vVirusMd5;

	HANDLE m_hScanThread;
	BOOL m_isScanning;

	virtual BOOL OnInitDialog();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedScan();
protected:
	afx_msg LRESULT FOUNDFILE(WPARAM wParam, LPARAM lParam);
public:
	CButton m_selectAll;

	/*
	*	Select all files.
	*/
	afx_msg void OnBnClickedSelectall();

	/*
	*	Delete selected files.
	*/
	afx_msg void OnBnClickedBtnDel();

	/*
	*	Clear useless visual studio files.
	*/
	afx_msg void OnBnClickedVsClear();

	/*
	*	Scan files without filter.
	*/
	afx_msg void OnBnClickedBtnscanall();
};
