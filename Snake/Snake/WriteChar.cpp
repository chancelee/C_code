#include "WriteChar.h"

void WriteChar(int nX, int nY, const char* str, WORD wAttributes)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD pos = { nX * 2,nY };

	GetConsoleScreenBufferInfo(hOut, &csbi);

	SetConsoleCursorPosition(hOut, pos);

	if (wAttributes != 0)
	{
		SetConsoleTextAttribute(hOut, wAttributes);
	}

	if (str != nullptr)
	{
		printf(str);
	}

	SetConsoleTextAttribute(hOut, csbi.wAttributes);

}


void WriteInt(int nX, int nY, int nNum, WORD wAttributes)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD pos = { nX * 2,nY };

	GetConsoleScreenBufferInfo(hOut, &csbi);

	SetConsoleCursorPosition(hOut, pos);

	if (wAttributes != 0)
	{
		SetConsoleTextAttribute(hOut, wAttributes);
	}

	printf("%d",nNum);

	SetConsoleTextAttribute(hOut, csbi.wAttributes);

}


