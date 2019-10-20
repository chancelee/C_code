#include <windows.h>
#include <stdio.h>
#define HOOKLEN		12

BYTE g_newCodes[HOOKLEN] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xC3 };
BYTE g_oldCodes[HOOKLEN] = { 0 };

extern "C"
__declspec(dllexport)
void InstallHook();

extern "C"
__declspec(dllexport)
void UninstallHook();

typedef HANDLE (WINAPI *POpenProcess)(
	_In_ DWORD dwDesiredAccess,
	_In_ BOOL bInheritHandle,
	_In_ DWORD dwProcessId
);


HANDLE WINAPI MyOpenProcess(
_In_ DWORD dwDesiredAccess,
_In_ BOOL bInheritHandle,
_In_ DWORD dwProcessId
)
{
	DWORD dwPID = 0;
	HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"PID");
	if (NULL == hMap || INVALID_HANDLE_VALUE == hMap)
	{
		return NULL;
	}
	LPVOID lpBuf = MapViewOfFile(
		hMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(DWORD));
	if (NULL == lpBuf)
	{
		return FALSE;
	}

	memcpy(&dwPID, lpBuf, sizeof(DWORD));

	// �������Ľ��� ֱ�ӷ��ش���
	if (dwProcessId == dwPID)
	{
		// ����ʧ��
		MessageBoxW(NULL, L"You can't kill me", L"Ooops", 0);
		return FALSE;
	}
	// ж�ع���
	UninstallHook();
	// ����ԭʼ����
	HANDLE handle = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
	// ���¹���
	InstallHook();
	// �����������ú�ľ��
	return handle;
	return NULL;
}


extern "C"
__declspec(dllexport)
void InstallHook()
{
	DWORD dwProtect = 0;

	memcpy(g_oldCodes, OpenProcess, HOOKLEN);

	VirtualProtectEx(GetCurrentProcess(), OpenProcess, HOOKLEN, PAGE_EXECUTE_READWRITE, &dwProtect);

	*(DWORD64*)&g_newCodes[2] = (DWORD64)MyOpenProcess;
	memcpy(OpenProcess, g_newCodes, HOOKLEN);

	VirtualProtectEx(GetCurrentProcess(), OpenProcess, HOOKLEN, dwProtect, &dwProtect);
}

extern "C"
__declspec(dllexport)
void UninstallHook()
{
	DWORD dwProtect = 0;
	VirtualProtect(OpenProcess, HOOKLEN, PAGE_EXECUTE_READWRITE, &dwProtect);
	memcpy(OpenProcess, g_oldCodes, HOOKLEN);
	VirtualProtect(OpenProcess, HOOKLEN, dwProtect, &dwProtect);
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		InstallHook();
		break;
	case DLL_PROCESS_DETACH:
		UninstallHook();
		break;
	default:
		break;
	}
	return TRUE;
}


