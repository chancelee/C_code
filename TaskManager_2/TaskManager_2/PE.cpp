// PE.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskManager_2.h"
#include "PE.h"
#include "afxdialogex.h"


// CPE 对话框

IMPLEMENT_DYNAMIC(CPE, CDialogEx)

CPE::CPE(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE, pParent)
	, m_pFile(NULL)
	, m_pDosHeader(NULL)
	, m_pNtHeader(NULL)
	, m_pFileHeader(NULL)
	, m_pOptHeader32(NULL)
	, m_pOptHeader64(NULL)
	, m_pFirstSection(NULL)
	, m_pIED(NULL)
	, m_pIID(NULL)
	, m_pRes(NULL)
	, m_pReloc(NULL)
	, m_pTLS32(NULL)
	,m_pTLS64(NULL)
	, m_pDelay(NULL)
	, m_strPathValue(_T(""))
{

}

CPE::~CPE()
{
	freePE();
}

void CPE::freePE()
{
	delete[] m_pFile;
	m_pFile = NULL;
	m_pDosHeader = NULL;
	m_pNtHeader = NULL;
	m_pFileHeader = NULL;
	m_pOptHeader32 = NULL;
	m_pOptHeader64 = NULL;
	m_pFirstSection = NULL;
	m_pIED = NULL;
	m_pIID = NULL;
	m_pRes = NULL;
	m_pReloc = NULL;
	m_pTLS32 = NULL;
	m_pTLS64 = NULL;
	m_pDelay = NULL;
}

void CPE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPathValue);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CPE, CDialogEx)
	ON_BN_CLICKED(IDC_OPENPE, &CPE::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_FILEHEAD, &CPE::OnBnClickedPemore)
	ON_BN_CLICKED(IDC_BTNOPT, &CPE::OnBnClickedBtnopt)
	ON_BN_CLICKED(IDC_EXP, &CPE::OnBnClickedExp)
	ON_BN_CLICKED(IDC_IMP, &CPE::OnBnClickedImp)
	ON_BN_CLICKED(IDC_RES, &CPE::OnBnClickedRes)
	ON_BN_CLICKED(IDC_RELOC, &CPE::OnBnClickedReloc)
	ON_BN_CLICKED(IDC_TLS, &CPE::OnBnClickedTls)
	ON_BN_CLICKED(IDC_DELAY, &CPE::OnBnClickedDelay)
	ON_BN_CLICKED(IDC_BTNSECHEAD, &CPE::OnBnClickedBtnsechead)
	ON_BN_CLICKED(IDC_BTNDATADIR, &CPE::OnBnClickedBtndatadir)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


#include "PE.h"


ULONGLONG	CPE::RVA2FO(ULONGLONG dwRVA)
{
	PIMAGE_SECTION_HEADER pSectionHeader = m_pFirstSection;

	if (dwRVA == 0)
	{
		return 0;
	}

	while (!(dwRVA >= pSectionHeader->VirtualAddress
		&& dwRVA < pSectionHeader->VirtualAddress + pSectionHeader->SizeOfRawData))
	{
		++pSectionHeader;
		/*if (pSectionHeader->SizeOfRawData == 0)
		{
		return -1;
		}*/
	}
	return pSectionHeader->PointerToRawData
		+ (dwRVA - pSectionHeader->VirtualAddress);
}



bool CPE::bIsPe()
{
	if (m_pDosHeader->e_magic == IMAGE_DOS_SIGNATURE
		&& m_pNtHeader->Signature == IMAGE_NT_SIGNATURE)
	{
		return true;
	}
	return false;
}




/*
*	Functions to show PE info.
*/
//void CPE::showDosHeader()
//{
//}
//
//void CPE::showNtHeader()
//{
//}

void CPE::OnBnClickedPemore()
{
	CString buf;
	char* characteristics[] = {
		"IMAGE_FILE_RELOCS_STRIPPED			 ",
		"IMAGE_FILE_EXECUTABLE_IMAGE		 ",
		"IMAGE_FILE_LINE_NUMS_STRIPPED		 ",
		"IMAGE_FILE_LOCAL_SYMS_STRIPPED		 ",
		"IMAGE_FILE_AGGRESIVE_WS_TRIM		 ",
		"IMAGE_FILE_LARGE_ADDRESS_AWARE		 ",
		"",
		"IMAGE_FILE_BYTES_REVERSED_LO		 ",
		"IMAGE_FILE_32BIT_MACHINE			 ",
		"IMAGE_FILE_DEBUG_STRIPPED			 ",
		"IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP	 ",
		"IMAGE_FILE_NET_RUN_FROM_SWAP		 ",
		"IMAGE_FILE_SYSTEM					 ",
		"IMAGE_FILE_DLL						 ",
		"IMAGE_FILE_UP_SYSTEM_ONLY			 ",
		"IMAGE_FILE_BYTES_REVERSED_HI		 "
	};

	if (!m_pFileHeader) return;
	
	buf.Format(L"NumberOfSections:	%04x\n"
			L"SizeOfOptionalHeader:	%04x\n"
			L"Characteristics:	%04x\n",
		m_pFileHeader->NumberOfSections,
		m_pFileHeader->SizeOfOptionalHeader,
		m_pFileHeader->Characteristics);
	for (BYTE i = 0; i < 16; ++i)
	{
		if (m_pFileHeader->Characteristics & (1 << i))
		{
			/*printf("\t");
			printf(characteristics[i]);
			printf("\n");*/
			buf += L"    ";
			buf += characteristics[i];
			buf += L"\n";
		}
	}
	MessageBoxW(buf, L"More File head info.");
}


void CPE::OnBnClickedBtnopt()
{
	CString buf;
	char* dllcharacteristics[] = {
		"",
		"",
		"",
		"",
		"",
		"IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA	0x0020	Image can handle a high entropy 64-bit virtual address space.",
		"IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE	0x0040	DLL can move.",
		"IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY	0x0080	Code Integrity Image",
		"IMAGE_DLLCHARACTERISTICS_NX_COMPAT	0x0100	Image is NX compatible",
		"IMAGE_DLLCHARACTERISTICS_NO_ISOLATION	0x0200	Image understands isolation and doesn't want it",
		"IMAGE_DLLCHARACTERISTICS_NO_SEH	0x0400	Image does not use SEH.  No SE handler may reside in this image ",
		"IMAGE_DLLCHARACTERISTICS_NO_BIND	0x0800	Do not bind this image.",
		"IMAGE_DLLCHARACTERISTICS_APPCONTAINER	0x1000	Image should execute in an AppContainer",
		"IMAGE_DLLCHARACTERISTICS_WDM_DRIVER	0x2000	Driver uses WDM model",
		"IMAGE_DLLCHARACTERISTICS_GUARD_CF	0x4000	Image supports Control Flow Guard.",
		"IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE	0x8000"
	};

	if (m_pOptHeader32)
	{
		buf.Format(L"SectionAlignment:	%08x\n"
			L"FileAlignment:	%08x\n"
			L"SizeOfImage:	%08x\n"
			L"SizeOfHeaders:	%08x\n"
			L"CheckSum:		%08x\n"
			L"DllCharacteristics:		%08x\n",
			m_pOptHeader32->SectionAlignment,
			m_pOptHeader32->FileAlignment,
			m_pOptHeader32->SizeOfImage,
			m_pOptHeader32->SizeOfHeaders,
			m_pOptHeader32->CheckSum,
			m_pOptHeader32->DllCharacteristics);

		for (BYTE i = 0; i < 16; ++i)
		{
			if (m_pOptHeader32->DllCharacteristics & (1 << i))
			{
			/*	printf("\t");
				printf(dllcharacteristics[i]);
				printf("\n");*/
				buf += L"    ";
				buf += dllcharacteristics[i];
				buf += L"\n";
			}

		}
		MessageBoxW(buf, L"More Opt head info.");
	}


	if (m_pOptHeader64)
	{
		buf.Format(L"SectionAlignment:	%08x\n"
			L"FileAlignment:	%08x\n"
			L"SizeOfImage:	%08x\n"
			L"SizeOfHeaders:	%08x\n"
			L"CheckSum:		%08x\n"
			L"DllCharacteristics:		%08x\n",
			m_pOptHeader64->SectionAlignment,
			m_pOptHeader64->FileAlignment,
			m_pOptHeader64->SizeOfImage,
			m_pOptHeader64->SizeOfHeaders,
			m_pOptHeader64->CheckSum,
			m_pOptHeader64->DllCharacteristics);

		for (BYTE i = 0; i < 16; ++i)
		{
			if (m_pOptHeader64->DllCharacteristics & (1 << i))
			{
				/*	printf("\t");
				printf(dllcharacteristics[i]);
				printf("\n");*/
				buf += L"    ";
				buf += dllcharacteristics[i];
				buf += L"\n";
			}

		}
		MessageBoxW(buf, L"More Opt head info.");
	}
	
}

/*
*	Section head format:
*		section name	:	RVA	: V size	: R offset	: R size	:	characteristics
*/
void CPE::OnBnClickedBtnsechead()
{
	if (!m_pFirstSection || !m_pFile) return;

	PIMAGE_SECTION_HEADER pSectionHeader = m_pFirstSection;
	CStringA bufA;
	CStringW bufW;

	/*char* characteristics[] = {

	};
	*/

	LVCOLUMNW column;
	unsigned int nRow = 0;

	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"section name";
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"RVA";
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"V size";
	m_listCtrl.SetColumn(2, &column);
	column.pszText = L"R offset";
	m_listCtrl.SetColumn(3, &column);
	column.pszText = L"R size";
	m_listCtrl.SetColumn(4, &column);
	column.pszText = L"characteristics";
	m_listCtrl.SetColumn(5, &column);

	m_listCtrl.DeleteAllItems();

	for (int i = 0; i < m_pFileHeader->NumberOfSections;
		++i, ++pSectionHeader, ++nRow)
	{
		wprintf(L"	name:	%s\n", pSectionHeader->Name);
		wprintf(L"	VirtualAddress:		0x%08x\n", pSectionHeader->VirtualAddress);
		wprintf(L"	V size:		0x%08x\n", pSectionHeader->Misc.VirtualSize);
		wprintf(L"	size of raw data:	0x%08x\n", pSectionHeader->SizeOfRawData);
		wprintf(L"	PointerToRawData:	0x%08x\n", pSectionHeader->PointerToRawData);
		wprintf(L"	Characteristics:	0x%08x\n", pSectionHeader->Characteristics);

		m_listCtrl.InsertItem(nRow, L"");

		bufA.Format("%s", pSectionHeader->Name);
		bufW = bufA;
		m_listCtrl.SetItemText(nRow, 0, bufW);

		bufW.Format(L"0x%08x", pSectionHeader->VirtualAddress);
		m_listCtrl.SetItemText(nRow, 1, bufW);
		bufW.Format(L"0x%08x", pSectionHeader->Misc.VirtualSize);
		m_listCtrl.SetItemText(nRow, 2, bufW);
		bufW.Format(L"0x%08x", pSectionHeader->PointerToRawData);
		m_listCtrl.SetItemText(nRow, 3, bufW);
		bufW.Format(L"0x%08x", pSectionHeader->SizeOfRawData);
		m_listCtrl.SetItemText(nRow, 4, bufW);
		bufW.Format(L"0x%08x", pSectionHeader->Characteristics);
		m_listCtrl.SetItemText(nRow, 5, bufW);
		
	}
}


/*
*	Export format:
*		dll name	:	ordinal		:	function name	:	RVA
*/
void CPE::OnBnClickedExp()
{
	if (!m_pFile) return;
	if (!m_pIED || !(m_pFileHeader->Characteristics & IMAGE_FILE_DLL))
	{
		MessageBoxW(L"No DLL file.");
		return;
	}

	PIMAGE_EXPORT_DIRECTORY pExportTab = m_pIED;

	DWORD *pFuncAddr = (DWORD *)(RVA2FO(pExportTab->AddressOfFunctions) + (ULONGLONG)m_pFile);
	DWORD *pFuncName = (DWORD *)(RVA2FO(pExportTab->AddressOfNames) + (ULONGLONG)m_pFile);
	WORD *pFuncOrd = (WORD *)(RVA2FO(pExportTab->AddressOfNameOrdinals) + (ULONGLONG)m_pFile);

	DWORD cNames = pExportTab->NumberOfNames;
	DWORD cFuncs = pExportTab->NumberOfFunctions;

	CStringW bufW;
	CStringA bufA;

	unsigned int nRow = 0;
	LVCOLUMNW column;

	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"dll name";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"Ordinal";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"func name";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(2, &column);
	column.pszText = L"RVA";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(3, &column);

	m_listCtrl.DeleteAllItems();
	
	//printf("DLL name:	%s\n", RVA2FO(pExportTab->Name) + (ULONGLONG)m_pFile);

	bufA.Format("%s", RVA2FO(pExportTab->Name) + (ULONGLONG)m_pFile);
	bufW = bufA;
	m_listCtrl.InsertItem(nRow, L"");
	m_listCtrl.SetItemText(nRow++, 0, bufW);

	for (int nIndexAddr = 0; nIndexAddr < cFuncs; ++nIndexAddr)
	{
		for (int nIndexOrd = 0; nIndexOrd < cNames; ++nIndexOrd)
		{
			if (nIndexAddr == pFuncOrd[nIndexOrd])
			{
				/*printf("	Ordinal:	%d		FuncNanme:		%s		FuncAddr:	0x%08x\n",
					nIndexOrd, RVA2FO(pFuncName[nIndexOrd]) + (ULONGLONG)m_pFile, pFuncAddr[nIndexAddr]);*/

				m_listCtrl.InsertItem(nRow, L"");
				bufW.Format(L"%d", nIndexOrd);
				m_listCtrl.SetItemText(nRow, 1, bufW);
				//CHAR_TO_WCHAR((LPCCH)(RVA2FO(pFuncName[nIndexOrd]) + (ULONGLONG)m_pFile), pWchar, nWchar);
				bufA.Format("%s", RVA2FO(pFuncName[nIndexOrd]) + (ULONGLONG)m_pFile);
				bufW = bufA;
				m_listCtrl.SetItemText(nRow, 2, bufW);
				bufW.Format(L"0x%08x", pFuncAddr[nIndexAddr]);
				m_listCtrl.SetItemText(nRow, 3, bufW);
				++nRow;

				break;
			}
			else if (nIndexOrd == cNames - 1)
			{
				//printf("	FuncAddr:		0x%08x\n", pFuncAddr[nIndexAddr]);
				bufW.Format(L"0x%08x", pFuncAddr[nIndexAddr]);
				m_listCtrl.SetItemText(nRow, 3, bufW);
				++nRow;
			}
		}
	}
}

/*
*	Import format:
*		dll name	:	ordinal		:	function name	
*/
void CPE::OnBnClickedImp()
{
	if (!m_pFile || !m_pIID) return;
	PIMAGE_IMPORT_DESCRIPTOR pImpTab = m_pIID;

	PIMAGE_THUNK_DATA32 pINT32 = NULL, pIAT32 = NULL;
	PIMAGE_THUNK_DATA64 pINT64 = NULL, pIAT64 = NULL;

	CStringW bufW;
	CStringA bufA;
	unsigned int nRow = 0;
	LVCOLUMNW column;


	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"dll name";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"Ordinal";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"func name";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(2, &column);


	m_listCtrl.DeleteAllItems();

	while (pImpTab->Name)
	{

		bufA.Format("%s", RVA2FO(pImpTab->Name) + (ULONGLONG)m_pFile);
		bufW = bufA;
		m_listCtrl.InsertItem(nRow, L"");
		m_listCtrl.SetItemText(nRow, 0, bufW);
		++nRow;
		
		if (m_bIs64)
		{
			pIAT64 = (PIMAGE_THUNK_DATA64)(RVA2FO(pImpTab->FirstThunk) + (ULONGLONG)m_pFile);
			while (pIAT64->u1.Ordinal)
			{
				if (pIAT64->u1.Ordinal
					& 1 << (8 * sizeof(pIAT64->u1.Ordinal) - 1))
				{
					//printf("	ordinal:	%04x\n", pIAT64->u1.Ordinal & ~(1 << (8 * sizeof(pIAT64->u1.Ordinal) - 1)));
					bufW.Format(L"%04x", pIAT64->u1.Ordinal & ~(1 << (8 * sizeof(pIAT64->u1.Ordinal) - 1)));
					m_listCtrl.InsertItem(nRow, L"");
					m_listCtrl.SetItemText(nRow, 1, bufW);
					++nRow;
				}
				else
				{
					PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(RVA2FO(pIAT64->u1.AddressOfData) + (ULONGLONG)m_pFile);
					//printf("	ordinal:	%04x		func name:		%s\n", pName->Hint, pName->Name);
					bufW.Format(L"%04x", pName->Hint);
					m_listCtrl.InsertItem(nRow, L"");
					m_listCtrl.SetItemText(nRow, 1, bufW);

					bufA.Format("%s", pName->Name);
					bufW = bufA;
					m_listCtrl.SetItemText(nRow, 2, bufW);
					++nRow;
				}

				++pIAT64;
			}
		}
		else
		{
			pIAT32 = (PIMAGE_THUNK_DATA32)(RVA2FO(pImpTab->FirstThunk) + (ULONGLONG)m_pFile);
			while (pIAT32->u1.Ordinal)
			{
				if (pIAT32->u1.Ordinal
					& 1 << (8 * sizeof(pIAT32->u1.Ordinal) - 1))
				{
					//printf("	ordinal:	%04x\n", pIAT32->u1.Ordinal & ~(1 << (8 * sizeof(pIAT32->u1.Ordinal) - 1)));
					bufW.Format(L"%04x", pIAT32->u1.Ordinal & ~(1 << (8 * sizeof(pIAT32->u1.Ordinal) - 1)));
					m_listCtrl.InsertItem(nRow, L"");
					m_listCtrl.SetItemText(nRow, 1, bufW);
					++nRow;
				}
				else
				{
					PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(RVA2FO(pIAT32->u1.AddressOfData) + (ULONGLONG)m_pFile);
					//printf("	ordinal:	%04x		func name:		%s\n", pName->Hint, pName->Name);
					bufW.Format(L"%04x", pName->Hint);
					m_listCtrl.InsertItem(nRow, L"");
					m_listCtrl.SetItemText(nRow, 1, bufW);

					bufA.Format("%s", pName->Name);
					bufW = bufA;
					m_listCtrl.SetItemText(nRow, 2, bufW);
					++nRow;
				}

				++pIAT32;
			}
		}

		++pImpTab;
	}
}

/*
*	Resource format:
*		res type	:	name	:	language	:	RVA	:	size	
*/
void CPE::OnBnClickedRes()
{
	if (!m_pFile || !m_pRes) return;

	PIMAGE_RESOURCE_DIRECTORY pRes_1 = m_pRes, pRes_2 = NULL, pRes_3 = NULL;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry_1 = NULL,
		pResEntry_2 = NULL,
		pResEntry_3 = NULL;

	DWORD dwEntry_1 = 0,
		dwEntry_2 = 0,
		dwEntry_3 = 0;
	PIMAGE_RESOURCE_DIR_STRING_U pName = NULL;
	PIMAGE_RESOURCE_DATA_ENTRY pData = NULL;

	char *aResTypes[] = {
		"",
		"cursor", "bitmap",
		"icon", "menu",
		"dialog", "string table",
		"font directory", "font",
		"accelerator", "unformatted",
		"message table", "group cursor",
		"",
		"group icon",
		"",
		"version",
	};


	CStringW bufW;
	CStringA bufA;
	unsigned int nRow = 0;
	LVCOLUMNW column;


	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"Res type";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"ID";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"Language";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(2, &column);
	column.pszText = L"RVA";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(3, &column);
	column.pszText = L"Size";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(4, &column);

	m_listCtrl.DeleteAllItems();

	pRes_1 = m_pRes;
	dwEntry_1 = pRes_1->NumberOfIdEntries + pRes_1->NumberOfNamedEntries;
	if (!dwEntry_1)
	{
		MessageBoxW(L"No resource\n");
		return;
	}

	/*
	*	Layer 1
	*/
	pResEntry_1 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes_1 + 1);
	for (int i = 0; i < dwEntry_1; ++i)
	{

		/*
		*	Layer 2
		*/
		if (pResEntry_1[i].DataIsDirectory)
		{
			pRes_2 = (PIMAGE_RESOURCE_DIRECTORY)
				(pResEntry_1[i].OffsetToDirectory + (ULONGLONG)pRes_1);
			dwEntry_2 = pRes_2->NumberOfIdEntries + pRes_2->NumberOfNamedEntries;

			/*printf("\t%d %s resources .\n"
				"\t%d hava name\n",
				dwEntry_2,
				pResEntry_1[i].Id <= 0x10 ? aResTypes[pResEntry_1[i].Name] : "Unknown type",
				pRes_2->NumberOfNamedEntries);*/


			/*
			*	Layer 3
			*/
			pResEntry_2 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes_2 + 1);
			for (int j = 0; j < dwEntry_2; ++j)
			{

				if (pResEntry_2[j].DataIsDirectory)
				{
					pRes_3 = (PIMAGE_RESOURCE_DIRECTORY)
						(pResEntry_2[j].OffsetToDirectory + (ULONGLONG)pRes_1);
					dwEntry_3 = pRes_3->NumberOfIdEntries + pRes_3->NumberOfNamedEntries;


					/*
					*	Data layer
					*/
					pResEntry_3 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes_3 + 1);
					for (int k = 0; k < dwEntry_3; ++k)
					{
						pData = (PIMAGE_RESOURCE_DATA_ENTRY)(pResEntry_3[k].OffsetToData + (ULONGLONG)pRes_1);

						bufA.Format("%s", pResEntry_1[i].Id <= 0x10 ? aResTypes[pResEntry_1[i].Id] : "Unknown type");
						bufW = bufA;
						m_listCtrl.InsertItem(nRow, L"");
						m_listCtrl.SetItemText(nRow, 0, bufW);

						if (pResEntry_2[j].NameIsString)
						{
							pName = (PIMAGE_RESOURCE_DIR_STRING_U)(pResEntry_2[j].NameOffset + pRes_1);
							bufA.Format("%s", pName->NameString);
							bufW = bufA;
							m_listCtrl.SetItemText(nRow, 1, bufW);

						}
						else
						{
							bufW.Format(L"%08x", pResEntry_2[j].Name);
							m_listCtrl.SetItemText(nRow, 1, bufW);
						}

						bufW.Format(L"%08x", pResEntry_3[k].Name);
						m_listCtrl.SetItemText(nRow, 2, bufW);

						bufW.Format(L"0x%08x", pData->OffsetToData);
						m_listCtrl.SetItemText(nRow, 3, bufW);

						bufW.Format(L"0x%08x", pData->Size);
						m_listCtrl.SetItemText(nRow, 4, bufW);

						++nRow;
					}
				}
			}

		}
	}
}

/*
*	Reloc format:
*		Block RVA	:	Num		:	RVA	:	File offset
*/
void CPE::OnBnClickedReloc()
{

	if (!m_pFile)
	{
		return;
	}
	if (!m_pReloc)
	{
		MessageBoxW(L"No relocation");
		return;
	}
	typedef struct {
		WORD	offset : 12;
		WORD	type : 4;
	}TypeOffset, *PTypeOffset;

	PIMAGE_BASE_RELOCATION pReloc = m_pReloc;
	DWORD dwCount = ((pReloc->SizeOfBlock) - 8) / 2;

	PTypeOffset pTypeOffset = (PTypeOffset)(pReloc + 1);
	ULONGLONG RVA = 0;

	CStringW strBlockRVA, buf;
	unsigned int nRow = 0;
	LVCOLUMNW column;


	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"Block RVA";
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"Num";
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"RVA";
	m_listCtrl.SetColumn(2, &column);
	column.pszText = L"File Offset";
	m_listCtrl.SetColumn(3, &column);

	m_listCtrl.DeleteAllItems();

	while (pReloc->SizeOfBlock)
	{
		//printf("Block RVA: %08x\n", pReloc->VirtualAddress);
		for (int i = 0; i < dwCount;
			++i, ++nRow)
		{
			m_listCtrl.InsertItem(nRow, L"");
			strBlockRVA.Format(L"%08x", pReloc->VirtualAddress);

			RVA = 0;
			//if (pTypeOffset[i].type == IMAGE_REL_BASED_HIGHLOW)
			//{
				m_listCtrl.SetItemText(nRow, 0, strBlockRVA);
				RVA = pTypeOffset[i].offset + pReloc->VirtualAddress;
				
				buf.Format(L"%d", (pReloc->SizeOfBlock - 8) / 2);
				m_listCtrl.SetItemText(nRow, 1, buf);

				buf.Format(L"0x%08x", RVA);
				m_listCtrl.SetItemText(nRow, 2, buf);

				buf.Format(L"0x%08x", RVA2FO(RVA));
				m_listCtrl.SetItemText(nRow, 3, buf);

			//}
			//else if (pTypeOffset[i].type == IMAGE_REL_BASED_DIR64)
			//{
			//	RVA = pTypeOffset[i].offset + pReloc->VirtualAddress;
			//	printf("\tRVA : %016X\n\tFOA : %016X\n\n", RVA, RVA2FO(RVA));
			//}
		}
		pReloc = (PIMAGE_BASE_RELOCATION)((ULONGLONG)pReloc + pReloc->SizeOfBlock);
	}
}


/*
*	TLS format:
*		StartAddressOfRawData : EndAddressOfRawData : AddressOfCallBacks :
*/
void CPE::OnBnClickedTls()
{
	if (!m_pFile) return;
	if (!m_pTLS32 || !m_pTLS64)
	{
		MessageBoxW(L"No TLS.\n");
		return;
	}
	
	
	CStringW buf;
	unsigned int nRow = 0;
	LVCOLUMNW column;


	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"VA Of RawData";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"VA Of RawData";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"VO of Callbacks";
	column.cchTextMax = wcsnlen(column.pszText, 150);
	m_listCtrl.SetColumn(2, &column);

	m_listCtrl.DeleteAllItems();

	if (m_bIs64)
	{
		m_listCtrl.InsertItem(nRow, L"");
		buf.Format(L"0x%08x", m_pTLS64->StartAddressOfRawData);
		m_listCtrl.SetItemText(nRow, 0, buf);
		buf.Format(L"0x%08x", m_pTLS64->EndAddressOfRawData);
		m_listCtrl.SetItemText(nRow, 1, buf);
		buf.Format(L"0x%08x", m_pTLS64->AddressOfCallBacks);
		m_listCtrl.SetItemText(nRow, 2, buf);

	}
	else
	{
		m_listCtrl.InsertItem(nRow, L"");
		buf.Format(L"0x%08x", m_pTLS32->StartAddressOfRawData);
		m_listCtrl.SetItemText(nRow, 0, buf);
		buf.Format(L"0x%08x", m_pTLS32->EndAddressOfRawData);
		m_listCtrl.SetItemText(nRow, 1, buf);
		buf.Format(L"0x%08x", m_pTLS32->AddressOfCallBacks);
		m_listCtrl.SetItemText(nRow, 2, buf);
	}
	
}


/*
*	Delay format :
*		dll name	:	IAT	RVA	:	Func Name	:	VA to Func
*/
void CPE::OnBnClickedDelay()
{
	if (!m_pFile)	return;
	if(!m_pDelay)
	{
		MessageBoxW(L"No dalay-loaded dll.");
		return;
	}


	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = m_pDelay;
	PIMAGE_THUNK_DATA pIAT = NULL, pINT = NULL;
	PIMAGE_IMPORT_BY_NAME	pName = NULL;

	CStringA bufA;
	CStringW bufW;
	unsigned int nRow = 0;
	LVCOLUMNW column;

	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"dll name";
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"RVA to IAT";
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"VA of Func";
	m_listCtrl.SetColumn(2, &column);
	column.pszText = L"Func Name";
	m_listCtrl.SetColumn(3, &column);

	m_listCtrl.DeleteAllItems();

	while (pDelay->DllNameRVA)
	{
		bufA.Format("%s", RVA2FO(m_pDelay->DllNameRVA) + (ULONGLONG)m_pFile);
		bufW = bufA;
			
		m_listCtrl.InsertItem(nRow, L"");
		m_listCtrl.SetItemText(nRow, 0, bufW);

		pIAT = (PIMAGE_THUNK_DATA)(RVA2FO(m_pDelay->ImportAddressTableRVA) + (ULONGLONG)m_pFile);
		pINT = (PIMAGE_THUNK_DATA)(RVA2FO(m_pDelay->ImportNameTableRVA) + (ULONGLONG)m_pFile);

		bufW.Format(L"0x%08x", m_pDelay->ImportAddressTableRVA);
		m_listCtrl.SetItemText(nRow, 1, bufW);
		++nRow;

		while (pIAT->u1.Function)
		{
			bufW.Format(L"0x%08x", pIAT->u1.Function);
			m_listCtrl.InsertItem(nRow, L"");
			m_listCtrl.SetItemText(nRow, 2, bufW);

			if (pINT->u1.AddressOfData)
			{
				pName = (PIMAGE_IMPORT_BY_NAME)(RVA2FO(pINT->u1.AddressOfData) + (ULONGLONG)m_pFile);
				bufA.Format("%s", pName->Name);
				bufW = bufA;
				m_listCtrl.SetItemText(nRow, 3, bufW);
			}

			++nRow;
			++pIAT;
			++pINT;
		}
		++pDelay;
	}

}

/*
*	Data Directory format :
*		entry name	:	RVA	:	Size
*/
void CPE::OnBnClickedBtndatadir()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pFile) return;

	PIMAGE_DATA_DIRECTORY pDataDir = m_bIs64 ? m_pOptHeader64->DataDirectory
		: m_pOptHeader32->DataDirectory;

	wchar_t *szDataEntryDesc[] = {
		L"Export Directory",
		L"Import Directory",
		L"Resource Directory",
		L"Exception Directory",
		L"Security Directory",
		L"Base Relocation Table",
		L"Debug Directory",
		L"Architecture Specific Data",
		L"RVA of GP",
		L"TLS Directory",
		L"Load Configuration Directory",
		L"Bound Import Directory in headers",
		L"Import Address Table",
		L"Delay Load Import Descriptors",
		L"COM Runtime descriptor"
	};

	CStringW strW;
	unsigned int nRow = 0;
	LVCOLUMNW column;


	column.pszText = L"";
	for (int i = 0; i < m_nColumn; ++i)
	{
		m_listCtrl.SetColumn(i, &column);
	}
	column.pszText = L"Data Dir Entry";
	m_listCtrl.SetColumn(0, &column);
	column.pszText = L"RVA";
	m_listCtrl.SetColumn(1, &column);
	column.pszText = L"Size";
	m_listCtrl.SetColumn(2, &column);

	m_listCtrl.DeleteAllItems();

	for (DWORD i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES - 1; ++i, ++nRow)
	{
		m_listCtrl.InsertItem(nRow, L"");

		m_listCtrl.SetItemText(nRow, 0, szDataEntryDesc[i]);
		strW.Format(L"%08x", pDataDir[i].VirtualAddress);
		m_listCtrl.SetItemText(nRow, 1, strW);
		strW.Format(L"%08x", pDataDir[i].Size);
		m_listCtrl.SetItemText(nRow, 2, strW);
	}


}
// CPE 消息处理程序

void CPE::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnDropFiles(hDropInfo);
	TCHAR path[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, path, MAX_PATH);

	m_strPathValue = path;
	UpdateData(FALSE);
}

void CPE::OnBnClickedOpen()

{
	if (m_pFile)
	{
		freePE();
	}
	if (m_strPathValue.IsEmpty())
	{
		TCHAR path[MAX_PATH] = { 0 };
		BROWSEINFO browsInfo = { 0 };
		browsInfo.pidlRoot = 0;
		browsInfo.lpszTitle = _T("Please choose a PE file");
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
				m_strPathValue = path;
				UpdateData(FALSE);
			}
			::CoTaskMemFree(lpItemList);
		}
		else
		{
			return;
		}

	}
	
	HANDLE hFile = CreateFile(m_strPathValue, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBytesRW = 0;
	DWORD dwFileSize = 0;


	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxW(L"CreateFile() error\n");
		return;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	m_pFile = new char[dwFileSize]();
	ReadFile(hFile, m_pFile, dwFileSize, &dwBytesRW, NULL);


	if (m_pFile)
	{
		m_pDosHeader = (PIMAGE_DOS_HEADER)(m_pFile);
		m_pNtHeader = (PIMAGE_NT_HEADERS)((ULONGLONG)m_pFile + m_pDosHeader->e_lfanew);

		if (!bIsPe())
		{
			MessageBoxW(L"This is not a PE file.");
			return;
		}
		m_pFileHeader = (PIMAGE_FILE_HEADER)(&(m_pNtHeader->FileHeader));

		if (m_pFileHeader->Machine == 0x014c)
		{
			MessageBoxW(L"Open file successfully\nx86 program.");
			m_pOptHeader32 = (PIMAGE_OPTIONAL_HEADER32)(&(m_pNtHeader->OptionalHeader));
			m_bIs64 = FALSE;
		}
		else if (m_pFileHeader->Machine == 0x8664)
		{
			MessageBoxW(L"Open file successfully\nx64 program.");
			m_pOptHeader64 = (PIMAGE_OPTIONAL_HEADER64)(&(m_pNtHeader->OptionalHeader));
			m_bIs64 = TRUE;
		}



		m_pFirstSection = IMAGE_FIRST_SECTION(m_pNtHeader);

		/*
		*	Ensure data entry in option header..
		*/

		if (m_bIs64)
		/*
		*	64bit
		*/
		{

			/*
			*	Export table.
			*/
			if (m_pFileHeader->Characteristics & IMAGE_FILE_DLL)
			{
				m_pIED = (m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]).VirtualAddress ?
					(PIMAGE_EXPORT_DIRECTORY)(
						RVA2FO((m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]).VirtualAddress)
						+ (ULONGLONG)m_pFile)
					: NULL;

			}



			/*
			*	Import table.
			*/
			m_pIID = (m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]).VirtualAddress ?
				(PIMAGE_IMPORT_DESCRIPTOR)(
					RVA2FO((m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: 0;


			/*
			*	Resource.
			*/
			m_pRes = (m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]).VirtualAddress ?
				(PIMAGE_RESOURCE_DIRECTORY)(
					RVA2FO((m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: 0;

			/*
			*	Relocation
			*/
			m_pReloc = (m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]).VirtualAddress ?
				(PIMAGE_BASE_RELOCATION)(
					RVA2FO((m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: NULL;

			/*
			*	TLS
			*/
			m_pTLS64 = (m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS]).VirtualAddress ?
				(PIMAGE_TLS_DIRECTORY64)(
					RVA2FO((m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: NULL;


			/*
			*	Delay loaded dll
			*/
			m_pDelay = (m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT]).VirtualAddress ?
				(PIMAGE_DELAYLOAD_DESCRIPTOR)(
					RVA2FO((m_pOptHeader64->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: NULL;
		}

		else
		/*
		*	32bit
		*/
		{

			/*
			*	Export table.
			*/
			if (m_pFileHeader->Characteristics & IMAGE_FILE_DLL)
			{
				m_pIED = (m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]).VirtualAddress ?
					(PIMAGE_EXPORT_DIRECTORY)(
						RVA2FO((m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]).VirtualAddress)
						+ (ULONGLONG)m_pFile)
					: NULL;
			}


			/*
			*	Import table.
			*/
			m_pIID = (m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]).VirtualAddress ?
				(PIMAGE_IMPORT_DESCRIPTOR)(
					RVA2FO((m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: 0;


			/*
			*	Resource.
			*/
			m_pRes = (m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]).VirtualAddress ?
				(PIMAGE_RESOURCE_DIRECTORY)(
					RVA2FO((m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: 0;

			/*
			*	Relocation
			*/
			m_pReloc = (m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]).VirtualAddress ?
				(PIMAGE_BASE_RELOCATION)(
					RVA2FO((m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: NULL;


			/*
			*	TLS
			*/
			m_pTLS32 = (m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS]).VirtualAddress ?
				(PIMAGE_TLS_DIRECTORY32)(
					RVA2FO((m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: NULL;

			/*
			*	Delay loaded dll
			*/
			m_pDelay = (m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT]).VirtualAddress ?
				(PIMAGE_DELAYLOAD_DESCRIPTOR)(
					RVA2FO((m_pOptHeader32->DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT]).VirtualAddress)
					+ (ULONGLONG)m_pFile)
				: NULL;
		}
	}
}

BOOL CPE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwOldStyle = m_listCtrl.GetExtendedStyle();

	
	m_listCtrl.SetExtendedStyle(dwOldStyle|
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES);

	m_nColumn = 6;

	m_listCtrl.InsertColumn(0, L"", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(1, L"", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(2, L"", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(3, L"", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(4, L"", LVCFMT_CENTER, 100);
	m_listCtrl.InsertColumn(5, L"", LVCFMT_CENTER, 100);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




