#include "ctrl.h"


int g_nScore = 0;

void InitWindow()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;

	SMALL_RECT rc = { 0,0, 160 - 1, 30 - 1 };   // 重置窗口位置和大小，设置为50*15  
	SetConsoleWindowInfo(hOut, true, &rc);

	cci.bVisible = 0;
	cci.dwSize = sizeof(cci);

	SetConsoleCursorInfo(hOut, &cci);

	system("color 70");
}

void WelcomeAndOption(char &cOp)
{
	HANDLE hIn = nullptr;
	INPUT_RECORD InRec;
	COORD pos = { 0, 0 };
	DWORD dwTemp;


	char *szOptions[] = {
		"1. single mode.    ",
		"2. load the backup.",
		"3. draw a map      ",
		"4. load a map      "
	};

	WriteChar(0, 0, snakePic);
	WriteChar(39, 7, "click to choose:", COLOR_TEXT);
	WriteChar(40, 10, szOptions[0], COLOR_TEXT);
	WriteChar(40, 12, szOptions[1], COLOR_TEXT);
	WriteChar(40, 14, szOptions[2], COLOR_TEXT);
	WriteChar(40, 16, szOptions[3], COLOR_TEXT);


	hIn = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	cOp = -1;
	while (cOp == -1)
	{
		ReadConsoleInputW(hIn, &InRec, 1, &dwTemp);
		if (InRec.EventType == MOUSE_EVENT)
		{
			pos = InRec.Event.MouseEvent.dwMousePosition;
			if (FROM_LEFT_1ST_BUTTON_PRESSED == InRec.Event.MouseEvent.dwButtonState)
			{
				for (int i = 0; i < 4; i++)
				{
					if (pos.X >= 80 && pos.X <= 98 && pos.Y == (10 + i * 2))
					{
						cOp = i + 1;
						break;
					}
				}
			}
		}
	}

	system("cls");

}
void InfoBox()
{
	WriteChar(MAP_LEN, 2, "w/s/a/d move", COLOR_TEXT);
	WriteChar(MAP_LEN, 3, "p to pause", COLOR_TEXT);
	WriteChar(MAP_LEN, 4, "b to save", COLOR_TEXT);
	WriteChar(MAP_LEN, 10, "Score: ", COLOR_TEXT);
	WriteInt(MAP_LEN + 7, 10, g_nScore,COLOR_TEXT);
}

void ChooseSpeed(int &nInternal)
{
	HANDLE hIn = nullptr;
	INPUT_RECORD InRec;
	COORD pos = { 0, 0 };
	DWORD dwTemp;
	char cOp = 0;

	WriteChar(30, 10, "1. Easy  ", COLOR_TEXT);
	WriteChar(30, 12, "2. Normal", COLOR_TEXT);
	WriteChar(30, 14, "3. Hard  ", COLOR_TEXT);

	hIn = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	cOp = -1;
	while (cOp == -1)
	{
		ReadConsoleInputW(hIn, &InRec, 1, &dwTemp);
		if (InRec.EventType == MOUSE_EVENT)
		{
			pos = InRec.Event.MouseEvent.dwMousePosition;

			if (FROM_LEFT_1ST_BUTTON_PRESSED == InRec.Event.MouseEvent.dwButtonState)
			{
				for (int i = 0; i < 3; i++)
				{
					if (pos.X >= 60 && pos.X <= 68 && pos.Y == (10 + i * 2))
					{
						cOp = i + 1;
						break;
					}
				}
			}
		}
	}

	switch (cOp)
	{
	case 1:
		nInternal = 300;
		break;
	case 2:
		nInternal = 200;
		break;
	case 3:
		nInternal = 100;
		break;
	default:
		break;
	}
	system("cls");
}

void SingleBegin()
{
	GenMap();
	InfoBox();

	PlaySoundA("sound\\bgm.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);

	while (1)
	{
		GetPlayerInput();
		MoveSnake();
		if (ScoreOp()) break;
		Sleep(g_nInternal);
	}
}

void GetPlayerInput()
{
	char ch = UP;

	if (_kbhit())
	{
		ch = _getch();
	}
	else
	{
		ch = 0;
	}

	switch (ch)
	{
	case 'w':
		if (g_nDir == DOWN) break;
		g_nDir = UP;
		break;
	case 's':
		if (g_nDir == UP) break;
		g_nDir = DOWN;
		break;
	case 'a':
		if (g_nDir == RIGHT) break;
		g_nDir = LEFT;
		break;
	case 'd':
		if (g_nDir == LEFT) break;
		g_nDir = RIGHT;
		break;
	case 'p':		//pause
		_getch();
		break;
	case 'b':		//backup
		SaveBk();
		WriteChar(MAP_LEN, 7, "                        ");
		WriteChar(MAP_LEN, 7, "Saving...", COLOR_TEXT);
		Sleep(1000);
		WriteChar(MAP_LEN, 7, "Game saved successfully!", COLOR_TEXT);
		Sleep(1000);
		WriteChar(MAP_LEN, 7, "                        ");
		break;
	default:
		break;
	}
}

int ScoreOp()
{
	/*
		run into the obstacle
	*/
	if (g_map[g_vSnake[0].X][g_vSnake[0].Y] == OBSTACLE || inBody(g_vSnake[0].X, g_vSnake[0].Y, g_nBodyLen))
	{
		PlaySoundA("sound\\what.wav", NULL, SND_SYNC | SND_NOSTOP | SND_FILENAME | SND_NODEFAULT);
		WriteChar(MAP_LEN + 1, 6, "Ooops~~~",COLOR_TEXT);
		WriteChar(MAP_LEN + 1, 7, "Press enter to exit.", COLOR_TEXT);
		InitSnake();
		_getch();
		system("cls");
		return -1;
	}
	/*
		eat
	*/
	else if (g_map[g_vSnake[0].X][g_vSnake[0].Y] == FOOD)
	{
		g_nScore++;
		if (g_nScore % 10 == 0)
		{
			PlaySoundA("sound\\clap.wav", NULL, SND_ASYNC | SND_NOSTOP | SND_FILENAME | SND_NODEFAULT);

		}
		else if (g_nScore % 2 == 0)
		{
			int nTemp = rand() % nInfoNum;
			WriteChar(MAP_LEN + 1, 7, "                      ");
			WriteChar(MAP_LEN + 1, 7, info[nTemp], COLOR_TEXT);
			PlaySoundA(szSndPaths[nTemp], NULL, SND_ASYNC | SND_NOSTOP | SND_FILENAME | SND_NODEFAULT);
		}
		else
		{
			PlaySoundA("sound\\pwn.wav", NULL, SND_ASYNC | SND_NOSTOP | SND_FILENAME | SND_NODEFAULT);
		}

		g_map[g_vSnake[0].X][g_vSnake[0].Y] = LAND;
		g_nBodyLen++;

		WriteInt(MAP_LEN + 7, 10,g_nScore,COLOR_TEXT);

		CreateFood();
		

	}// get score end

	return 0;
}

void CreateFood()
{
	int nX, nY;
	COORD foodPos = { 0,0 };

	do
	{
		nX = rand() % (MAP_LEN - 2) + 1;
		nY = rand() % (MAP_WID - 2) + 1;
	} while (inBody(nX, nY, g_nBodyLen) || g_map[nX][nY] == OBSTACLE);

	g_map[nX][nY] = FOOD;

	WriteChar(nX, nY, "  ", COLOR_FOOD);
}

int SaveBk()
{
	time_t t;
	struct tm lt;
	char szPath[40] = { 0 };
	FILE *fp = nullptr;


	time(&t);
	localtime_s(&lt, &t);
	sprintf_s(szPath, 40, "backup\\%d%02d%02d%02d%02d%02d.bk", lt.tm_year + 1900, lt.tm_mon, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);
	
	system("mkdir backup 2>nul");

	fopen_s(&fp, szPath, "ab");
	if (fp == NULL)
	{
		puts("fopen_s() error\n");
		return -1;
	}

	fwrite(g_map, 1, sizeof(g_map), fp);
	fwrite(&g_nBodyLen, sizeof(int), 1, fp);
	fwrite(&g_vSnake[0], sizeof(COORD), g_nBodyLen, fp);
	fwrite(&g_nDir, 1, 1, fp);
	fwrite(&g_nScore, sizeof(int), 1, fp);
	fwrite(&g_nInternal, sizeof(int), 1, fp);



	fclose(fp);
	return 0;
}

int LoadBk()
{
	vector<char*> vpBk;
	long hFile = 0;
	struct _finddata_t fileInfo;
	string pathName;

	FILE *fp = nullptr;
	char szPath[40] = "backup\\";
	char cOp = -1;

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InRec;
	COORD pos = { 0, 0 };
	DWORD dwTemp;

	/*
		read the dir
	*/
	if ((hFile = _findfirst(pathName.assign(szPath).append("\\*.bk").c_str(), &fileInfo)) == -1) {
		WriteChar(30, 10, "No backup");
		Sleep(1000);
		system("cls");
		return -1;
	}
	do
	{
		char *buf = (char*)malloc(20);
		strcpy_s(buf,20,fileInfo.name);
		vpBk.push_back(buf);
	} while (_findnext(hFile, &fileInfo) == 0);

	/*
		output the .bk files
	*/
	WriteChar(30, 2, "Choose a backup:", COLOR_TEXT);
	for (int i = 0; i < vpBk.size(); i++)
	{
		WriteInt(30, 4 + i * 2 + 1, i + 1);
		WriteChar(32, 4 + i * 2 + 1, vpBk[i], COLOR_TEXT);
	}

	/*
		option
	*/
	SetConsoleMode(hIn,ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	while (cOp == -1)
	{
 		ReadConsoleInputW(hIn, &InRec, 1, &dwTemp);
		if (InRec.EventType == MOUSE_EVENT)
		{
			if (FROM_LEFT_1ST_BUTTON_PRESSED == InRec.Event.MouseEvent.dwButtonState)
			{
				pos = InRec.Event.MouseEvent.dwMousePosition;

				for (int i = 0; i < vpBk.size(); i++)
				{
					if (pos.X >= 60 && pos.X <= 80 && pos.Y == (4 + i * 2 + 1))
					{
						cOp = i;
						WriteInt(0, 7,cOp);
						break;
					}
				}
			}
		}
 	}

	/*
		open the .bk
	*/
	strcat_s(szPath,40,vpBk[cOp]);

	/*
		free malloc()
	*/
	while (vpBk.size())
	{
		vector<char*>::iterator it = vpBk.begin();
		free(*it);
		vpBk.erase(it);
	}

	fopen_s(&fp, szPath, "rb");
	if (fp == NULL)
	{
		puts("fopen_s() error\n");
		return -1;
	}


	while (g_vSnake.size())
	{
		vector<COORD>::iterator it = g_vSnake.begin();
		g_vSnake.erase(it);
	}

	fread(g_map, 1, sizeof(g_map), fp);
	fread(&g_nBodyLen, sizeof(int), 1, fp);

	for (int i = 0; i < g_nBodyLen; i++)
	{
		COORD temp = { 0,0 };
		g_vSnake.push_back(temp);
	}
	fread(&g_vSnake[0], sizeof(COORD), g_nBodyLen, fp);
	fread(&g_nDir, 1, 1, fp);
	fread(&g_nScore, sizeof(int), 1, fp);
	fread(&g_nInternal, sizeof(int), 1, fp);
	
	fclose(fp);
	system("cls");
	return 0;
}