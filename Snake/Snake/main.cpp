#include "ctrl.h"

int main()
{
	char cChoice = 0;

	SetConsoleTitleW(L"SNAKE");
	keybd_event(VK_SHIFT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

	InitWindow();

	while (1)
	{
		WelcomeAndOption(cChoice);
		switch (cChoice)
		{
		case 1:
			g_nScore = 0;
			ChooseSpeed(g_nInternal);
			InitMap();
			InitSnake();
			CreateFood();
			SingleBegin();
			break;
		case 2:
			if (LoadBk()) break;
			SingleBegin();
			break;
		case 3:
			DrawMap();
			break;
		case 4:
			if(LoadMap()) break;
			g_nScore = 0;
			ChooseSpeed(g_nInternal);
			InitSnake();
			CreateFood();
			SingleBegin();
			break;
		default:
			break;
		}
	}

	return 0;
}


