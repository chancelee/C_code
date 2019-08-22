#include "UI.h"



/*
	Write strings and numbers on the screen.
	The wAttr parameters are provided in Attr.h and Tank.h.
*/
void WriteStr(int x, int y, char *pStr, WORD wAttr)
{
	CONSOLE_CURSOR_INFO cci = { 1,FALSE };
	COORD pos = { x * 2, y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorInfo(hOut, &cci);
	SetConsoleTextAttribute(hOut, wAttr);
	SetConsoleCursorPosition(hOut, pos);

	printf(pStr);

}
void WriteInt(int x, int y, int nNum, WORD wAttr)
{
	CONSOLE_CURSOR_INFO cci = { 1,FALSE };
	COORD pos = { x * 2, y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorInfo(hOut, &cci);
	SetConsoleTextAttribute(hOut, wAttr);
	SetConsoleCursorPosition(hOut, pos);

	printf("%d",nNum);

}


/*
	Set the window's size, mouse's attribute, and screen color.
*/
void InitWindow()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	SMALL_RECT rc = { 0,0, WINDOW_LEN - 1, WINDOW_WID - 1 };

	keybd_event(VK_SHIFT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

	SetConsoleWindowInfo(hOut, TRUE, &rc);
	SetConsoleTitle(L"Tank War");

	cci.bVisible = FALSE;
	cci.dwSize = sizeof(cci);

	SetConsoleCursorInfo(hOut, &cci);

	SetWindowLongPtrA(
		GetConsoleWindow(),
		GWL_STYLE,
		GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE)
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX
	);

	system("color 80");

}

void WelcomeAndOption(char *pChoice)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD keyrec;
	DWORD res = 0;

	BOOL bSelected = FALSE;

	int x = WINDOW_LEN / 6, y = WINDOW_WID / 2 - 8;

	char *szOptions[] = {
		"                   ",
		"1. Start Game      ",
		"2. Load Backup     ",
		"3. Draw a Map      ",
		"4. Load a Map      "
	};

	*pChoice = START;

	WriteStr(4,y,pTankPic,COLOR_TEXT);
	WriteStr(x, y   ,  szOptions[1], COLOR_TEXT_SELECTED);
	WriteStr(x, y + 2, szOptions[2], COLOR_TEXT);
	WriteStr(x, y + 4, szOptions[3], COLOR_TEXT);
	WriteStr(x, y + 6, szOptions[4], COLOR_TEXT);

	while (!bSelected)
	{
		ReadConsoleInput(hIn, &keyrec, 1, &res);
		if (keyrec.EventType == KEY_EVENT
			&& keyrec.Event.KeyEvent.bKeyDown)
		{
			switch (keyrec.Event.KeyEvent.wVirtualKeyCode)
			{
			case VK_UP:
				if (*pChoice > 1)
				{
					WriteStr(x, y + 2 * (*pChoice - 1), szOptions[*pChoice], COLOR_TEXT);
					--*pChoice;
					WriteStr(x, y + 2 * (*pChoice - 1), szOptions[*pChoice], COLOR_TEXT_SELECTED);
				}
				break;
			case VK_DOWN:

				if (*pChoice < 4)
				{
					WriteStr(x, y + 2 * (*pChoice - 1), szOptions[*pChoice], COLOR_TEXT);
					++*pChoice;
					WriteStr(x, y + 2 * (*pChoice - 1), szOptions[*pChoice], COLOR_TEXT_SELECTED);
				}
				break;
			case VK_RETURN:
				bSelected = TRUE;
				break;
			default:
				break;
			}
		}
	}	

	SetConsoleTextAttribute(hOut,COLOR_LAND);
	system("cls");
}

char *pTankPic =
"                 ---- \n"
"                         |  | \n"
"                         |  | \n"
"                         |  | \n"
"                         |  | \n"
"                         |  | \n"
"                         |  | \n"
"        |----|           |  |           |----|\n"
"        |----|---------- |  | ----------|----|\n"
"        |----|           |  |           |----|\n"
"        |----|           |  |           |----|\n"
"        |----|           |  |           |----|\n"
"        |----|           |  |           |----|\n"
"        |----|           |  |           |----|\n"
"        |----|           |  |           |----|\n"
"        |----|       ------------       |----|\n"
"        |----|      |------------|      |----|\n"
"        |----|      |------------|      |----|\n"
"        |----|      |------------|      |----|\n"
"        |----|      |------------|      |----|\n"
"        |----|      |------------|      |----|\n"
"        |----|                          |----|\n"
"        |----|                          |----|\n"
"        |----|！！！！！！！！！！！！！|----|\n"
"        |----|                          |----|\n";