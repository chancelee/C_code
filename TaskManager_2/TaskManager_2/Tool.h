#pragma once

#include <stdio.h>
#include <windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>
#include <list>
#include <Winternl.h>

struct PROCESSINFO : public PROCESSENTRY32
{
	DWORD	dwMemoryUsage; // 内存使用率
	double	dCpuUsage = 0;// cpu使用率

};

class MYMODULEINFO
{
public:
	MODULEINFO stcModinfo;
	WCHAR *wszFileName;
public:
	MYMODULEINFO();
	~MYMODULEINFO();

	MYMODULEINFO(const MYMODULEINFO& obj);

};

class MYMEMINFO
{
public:
	MEMORYSTATUS  stcMemStatus;
	double cpuOccu;
};

struct FILEINFO {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	TCHAR path[MAX_PATH];
};

double FileTime2Double(const _FILETIME& fileTime);

void GetProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime);

BOOL GetProcesses(std::vector<PROCESSINFO>& processList);
double GetProcessMemoryUsage(DWORD dwPid);
double GetProcessCpuUsage(DWORD dwPid);


BOOL EnumThreads(DWORD dwPid, std::vector<THREADENTRY32> &v);

BOOL EnumModules96(DWORD dwPid, std::vector<MYMODULEINFO> &v);
BOOL EnumHeaps(DWORD dwPid);
BOOL CALLBACK EnumWindowProc(HWND hWnd, LPARAM lParam);
BOOL GetMemInfo(MYMEMINFO &meminfo);

bool findFiles(const TCHAR* dir, const TCHAR* filter, std::vector<PWIN32_FIND_DATA> *vFile);

