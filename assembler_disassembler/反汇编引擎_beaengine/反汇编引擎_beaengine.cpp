// ���������_beaengine.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "headers/BeaEngine.h"
#pragma comment(lib, "Win32/Lib/BeaEngine.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")

int main()
{

	BYTE bTest[] = { 0x68,0x37,0x31,0x40,0x00,0xFF,0x15,0x0C,0x20,0x40,0x00,0x83,0xC4,0x04,0xE8,0x0E,0x00,0x00,0x00,0xE8,0x3E,0x00,0x00,0x00,0xEB,0xE6,0x6A,0x00,0xE8,0x63,0x00,0x00 };
	// 1. ��ʼ�����������
	DISASM objDiasm;
	objDiasm.EIP = (UIntPtr)bTest; // ��ʼ��ַ
	objDiasm.VirtualAddr = (UINT64)0x400000; // �����ڴ��ַ��������������ڼ����ַ��
	objDiasm.Archi = 0;                      // AI-X86
	objDiasm.Options = MasmSyntax;           // MASM
	UINT uLen = Disasm(&objDiasm);
	CHAR szOpcode[32] = {};
	PCHAR pOpcode = szOpcode;
	for (UINT i = 0; i < uLen; i++) {
		sprintf_s(&szOpcode[2 * i], 20, "%02x", bTest[i]);
	}
	printf("%s:%s\n", szOpcode, objDiasm.CompleteInstr);
	return 0;
}
