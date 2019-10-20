#include <stdio.h>
#include "stdafx.h"
#include "Tool.h"
#include "Psapi.h"


// 初始化一个全局互斥体,在遍历文件时使用.
static HANDLE g_hMutex = []() -> HANDLE {
	return CreateMutex(0, FALSE, NULL);
}();

double FileTime2Double(const _FILETIME& fileTime)
{
	return double(fileTime.dwHighDateTime * 4.294967296e9)
		+ double(fileTime.dwLowDateTime);
}

BOOL GetProcesses(std::vector<PROCESSINFO>& processList)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPPROCESS,
		0);

	PROCESSENTRY32  stcPe = { sizeof(stcPe) };


	if (hSnap == INVALID_HANDLE_VALUE)
		return FALSE;

	if (Process32First(hSnap, &stcPe))
	{

		do {
			//wprintf(L"pid :%d   %ls\n", stcPe.th32ProcessID, stcPe.szExeFile);
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				FALSE, 
				stcPe.th32ProcessID);
			PROCESSINFO pi = *(PROCESSINFO*)&stcPe;
			if (hProcess)
			{
				PROCESS_MEMORY_COUNTERS pmc = {sizeof(PROCESS_MEMORY_COUNTERS)};
				pi.dwMemoryUsage = GetProcessMemoryUsage(pi.th32ProcessID);
				pi.dCpuUsage = GetProcessCpuUsage(pi.th32ProcessID);
				CloseHandle(hProcess);
			}
			else
			{
				pi.dwMemoryUsage = 0;
				pi.dCpuUsage = 0;
			}
			processList.push_back(pi);
		} while (Process32Next(hSnap, &stcPe));
	}

	CloseHandle(hSnap);
}
void GetProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime)
{
	typedef struct _KERNEL_USERTEXTIMES
	{
		LARGE_INTEGER CreateTime;
		LARGE_INTEGER ExitTime;
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
	} KERNEL_USERTEXTIMES, *PKERNEL_USERTEXTIMES;
	KERNEL_USERTEXTIMES time;
#define ProcessTimes  4

	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	if (hProc == NULL) {
		return;
	}
	decltype(NtQueryInformationProcess)* fnNtQueryInformationProcess;
	fnNtQueryInformationProcess = (decltype(NtQueryInformationProcess)*)
		(GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess"));

	DWORD read = 0;
	fnNtQueryInformationProcess(hProc,
		(PROCESSINFOCLASS)ProcessTimes,
		&time,
		sizeof(time),
		&read);
	*kernelTIme = time.KernelTime;
	*userTime = time.UserTime;
	CloseHandle(hProc);
}
double GetProcessCpuUsage(DWORD dwPid)
{
	// 公式: (进程内核时间 + 进程用户时间) / (CPU内核时间 + CPU用户时间)

	LARGE_INTEGER kernelTime = { 0 };
	LARGE_INTEGER userTime = { 0 };
	GetProcessTime(dwPid, &kernelTime, &userTime);

	//			空闲时间		内核时间		用户时间
	FILETIME cpuIdleTime = {}, cpuKernelTime = {}, cpuUserTime = {};

	// 获取时间
	GetSystemTimes((FILETIME*)&cpuIdleTime, (FILETIME*)&cpuKernelTime, (FILETIME*)&cpuUserTime);

	double proc = FileTime2Double(*(FILETIME*)&kernelTime) + FileTime2Double(*(FILETIME*)&userTime);
	double cpu = FileTime2Double(cpuKernelTime) + FileTime2Double(cpuKernelTime);
	return proc / cpu * 100.0;
}
double GetProcessMemoryUsage(DWORD dwPid)
{
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwPid);
	double usage = 0;
	if (hProc != NULL) {

		PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };;
		GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
		usage = pmc.WorkingSetSize;
		CloseHandle(hProc);
	}
	return usage / 1024;
}

MYMODULEINFO::MYMODULEINFO()
{
	wszFileName = new WCHAR[0x1000]();
}
MYMODULEINFO::~MYMODULEINFO()
{
	delete[] wszFileName;
}
MYMODULEINFO::MYMODULEINFO(const MYMODULEINFO& obj)
{
	stcModinfo = obj.stcModinfo;
	wszFileName = new WCHAR[0x1000]();
	memcpy(wszFileName, obj.wszFileName, 0x1000 * sizeof(WCHAR));
}

BOOL EnumThreads(DWORD dwPid, std::vector<THREADENTRY32> &v)
{
	HANDLE hSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		MessageBoxW(NULL, L"EnumThreads ERROR", L"error", 0);
		return FALSE;
	}

	te32.dwSize = sizeof(THREADENTRY32);

	if (Thread32First(hSnap, &te32))
	{
		do
		{
			if (te32.th32OwnerProcessID == dwPid)
			{
				//wprintf(L"TID:%d\n", te32.th32ThreadID);
				v.push_back(te32);
			}
		} while (Thread32Next(hSnap, &te32));
	}
	CloseHandle(hSnap);
	return TRUE;
}

//BOOL EnumModules32(DWORD dwPid)
//{
//	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
//	MODULEENTRY32 module32 = { 0 };
//
//	if (Module32First(hSnap, &module32))
//	{
//		do
//		{
//			//wprintf(L"%d - %ls", module32.th32ModuleID, module32.szModule);
//		} while (Module32Next(hSnap, &module32));
//	}
//	else
//	{
//		MessageBoxW(NULL, L"EnumModules32() ERROR", L"ERROR", 0);
//		return FALSE;
//	}
//	return TRUE;
//}

BOOL EnumModules96(DWORD dwPid, std::vector<MYMODULEINFO> &v)
{
	HANDLE hProcess = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwPid);
	HMODULE *hModules = new HMODULE[0x2000]();
	DWORD dwSize = 0, dwModuleCount = 0;

	MYMODULEINFO modinfo;

	if (hProcess == NULL)
	{
		MessageBoxW(NULL, L"EnumModules96() ERROR", L"ERROR", 0);
		return FALSE;
	}
	EnumProcessModulesEx(hProcess, hModules, sizeof(HMODULE) * 0x2000,
		&dwSize, LIST_MODULES_ALL);

	dwModuleCount = dwSize / sizeof(HMODULE);

	size_t i = 0;
	//while(hModules[i])
	for (; i < dwModuleCount; ++i)
	{
		ZeroMemory(modinfo.wszFileName, 0x1000 * sizeof(WCHAR));
		GetModuleFileNameEx(hProcess, hModules[i],
			modinfo.wszFileName, 0x1000);
		GetModuleInformation(hProcess, hModules[i],
			&modinfo.stcModinfo, sizeof(MODULEINFO));
		//wprintf(L"%ls\n", wszFileName);
		//wprintf(L"	EP:			%p\n", (WCHAR*)modinfo.EntryPoint);
		//wprintf(L"	BaseOfDll:	%p\n", (WCHAR*)modinfo.lpBaseOfDll);
		//wprintf(L"	SizeOfImage:%u\n", modinfo.SizeOfImage);
		v.push_back(modinfo);
	}

	delete[] hModules;
	
	return TRUE;
}
BOOL EnumHeaps(DWORD dwPid)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, dwPid);
	HEAPLIST32 hel32 = { 0 };
	if (Heap32ListFirst(hSnap, &hel32))
	{
		//wprintf(L"pid : %d\n", hel32.th32ProcessID);
		do
		{
			//wprintf(L"	HeapID: %d\n", hel32.th32HeapID);
		} while (Heap32ListNext(hSnap, &hel32));
	}
	return TRUE;
}

BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam)
{

	CListCtrl *listCtrl = (CListCtrl*)(lParam);

	TCHAR *pBuf = new TCHAR[0x1000];
	GetWindowTextW(hWnd, pBuf, 0x1000);
	if (IsWindowVisible(hWnd) && wcslen(pBuf))
	{
		listCtrl->InsertItem(0, L"");
		listCtrl->SetItemText(0, 0, pBuf);
		wsprintf(pBuf, L"%d", (DWORD)hWnd);
		listCtrl->SetItemText(0, 1, pBuf);
	}
	delete[] pBuf;
	return TRUE;
}


BOOL GetMemInfo(MYMEMINFO &meminfo)
{
	
	// memory info
	GlobalMemoryStatus(&meminfo.stcMemStatus);

	//wprintf(L"Mem loaded: %d%%\n", memStatus.dwMemoryLoad);
	//wprintf(L"phys used: %uMB\n", (memStatus.dwTotalPhys - memStatus.dwAvailPhys) / 1024 / 1024);

	// cpu rate
	_FILETIME oldIdleTime, oldKernelTime, oldUserTime;
	_FILETIME newIdleTime, newKernelTime, newUserTime;
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	GetSystemTimes(&oldIdleTime, &oldKernelTime, &oldUserTime);
	WaitForSingleObject(hEvent, 1000);
	GetSystemTimes(&newIdleTime, &newKernelTime, &newUserTime);

	double dOldIdleTime = FileTime2Double(oldIdleTime);
	double dNewIdleTime = FileTime2Double(newIdleTime);
	double dOldKernelTime = FileTime2Double(oldKernelTime);
	double dNewKernelTime = FileTime2Double(newKernelTime);
	double dOldUserTime = FileTime2Double(oldUserTime);
	double dNewUserTime = FileTime2Double(newUserTime);

	meminfo.cpuOccu = 100.0 -
		(dNewIdleTime - dOldIdleTime) /
		(dNewKernelTime - dOldKernelTime
			+ dNewUserTime - dOldUserTime)
		* 100.0;
	//wprintf(L"cpu occupancy: %.2lf%%\n", occupancy);
	return TRUE;
}


// 查找文件
bool findFiles(const TCHAR* dir, const TCHAR* filter, std::vector<PWIN32_FIND_DATA>* pVecFile)
{
	CString path = dir;
	PWIN32_FIND_DATA pFindData = new WIN32_FIND_DATA;
	CStringW strW;

	HANDLE hFind = FindFirstFile(path + _T("\\*"), pFindData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}

	if (!pVecFile) return FALSE;

	//FILEINFO *fInfo = new FILEINFO;
	do
	{
		if (_tcscmp(pFindData->cFileName, _T(".")) == 0 || _tcscmp(pFindData->cFileName, _T("..")) == 0) {
			continue;
		}
		// 是否是目录
		if (pFindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// 递归遍历下级目录
			findFiles(path + "\\" + pFindData->cFileName, filter, pVecFile);
		}
		else {

			/*
			*	If no filter, push back all files.
			*/
			if (!wcscmp(filter, L""))
			{
				strW.Format(L"%s\\%s", dir, pFindData->cFileName);

				_stprintf_s(pFindData->cFileName, _countof(pFindData->cFileName), strW);

				// 如果列表不为空,则将数据保存到列表
				WaitForSingleObject(g_hMutex, -1);
				pVecFile->push_back(pFindData);
				ReleaseMutex(g_hMutex);

				pFindData = new WIN32_FIND_DATA;
				continue;
			}

			// 获取文件扩展名
			TCHAR* pExtName = pFindData->cFileName + _tcslen(pFindData->cFileName);
			while (pExtName != pFindData->cFileName && *pExtName != '.')
				--pExtName;
			// 判断扩展名是否是要过滤出来的文件扩展名
			if (_tcsstr(filter, pExtName) != NULL) {

				// 拼接完整的路径
				strW.Format(L"%s\\%s", dir, pFindData->cFileName);

				_stprintf_s(pFindData->cFileName, _countof(pFindData->cFileName),strW);

				// 如果列表不为空,则将数据保存到列表
				WaitForSingleObject(g_hMutex, -1);
				pVecFile->push_back(pFindData);
				ReleaseMutex(g_hMutex);

				pFindData = new WIN32_FIND_DATA;
				
				//// 如果窗口句柄不为空, 则将数据发送到窗口
				//if (hWnd)
				//{
				//	SendMessage(hWnd, uMsg, (WPARAM)&pFindData, 0);
				//}
			}

		}
	} while (FindNextFile(hFind, pFindData));


	return true;
}




