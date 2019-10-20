#pragma once
#include "afxcmn.h"

/*
*	PE parser class.
*		It can parse 32/64 bits PE file.
*/


// CPE 对话框

class CPE : public CDialogEx
{
	DECLARE_DYNAMIC(CPE)

public:
	CPE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPE();
	void freePE();
	bool bIsPe();

	ULONGLONG RVA2FO(ULONGLONG dwRVA);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void *m_pFile;
	bool m_bIs64;
	int  m_nColumn;

	PIMAGE_DOS_HEADER m_pDosHeader;
	PIMAGE_NT_HEADERS m_pNtHeader;
	PIMAGE_FILE_HEADER m_pFileHeader;

	PIMAGE_OPTIONAL_HEADER32 m_pOptHeader32;
	PIMAGE_OPTIONAL_HEADER64 m_pOptHeader64;

	PIMAGE_SECTION_HEADER m_pFirstSection;

	PIMAGE_EXPORT_DIRECTORY m_pIED;
	PIMAGE_IMPORT_DESCRIPTOR m_pIID;

	PIMAGE_RESOURCE_DIRECTORY m_pRes;
	PIMAGE_BASE_RELOCATION	m_pReloc;

	PIMAGE_TLS_DIRECTORY32	m_pTLS32;
	PIMAGE_TLS_DIRECTORY64	m_pTLS64;

	PIMAGE_DELAYLOAD_DESCRIPTOR m_pDelay;
public:

	CString m_strPathValue;

	virtual BOOL OnInitDialog();
	CListCtrl m_listCtrl;

	/*
	*	Show more except static information.
	*/
	afx_msg void OnBnClickedPemore();
	
	/*
	* Open PE file button.
	*	This process can check "MZ" and "PE\0\0" signature. 
	*/
	afx_msg void OnBnClickedOpen();

	/*
	*	Button to show the optional header .
	*/
	afx_msg void OnBnClickedBtnopt();

	/*
	*	Show the data directory.
	*/
	afx_msg void OnBnClickedBtndatadir();


	/*
	*	Show the export table if any.
	*/
	afx_msg void OnBnClickedExp();

	/*
	*	Show the import table.
	*/
	afx_msg void OnBnClickedImp();

	/*
	*	Show the resource table.
	*/
	afx_msg void OnBnClickedRes();

	/*
	*	Show the relocation table.
	*/
	afx_msg void OnBnClickedReloc();

	/*
	*	Show the TLS table if any.
	*/
	afx_msg void OnBnClickedTls();

	/*
	*	Show the dalay dll table.
	*/
	afx_msg void OnBnClickedDelay();

	/*
	*	Show section headers.
	*/
	afx_msg void OnBnClickedBtnsechead();
	/*
	*	Drop PE files.
	*/
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
