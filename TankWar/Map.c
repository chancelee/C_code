#include "Map.h"

BYTE g_map[MAP_LEN][MAP_WID] = { LAND };

int g_playerBeginPosY = MAP_WID - 4;
int g_playerBeginPosX[2]= { MAP_LEN / 2 - 4 , MAP_LEN / 2 + 2 };
int g_enemyBeginPosY = 1;
int g_enemyBeginPosX[3] = { 1, MAP_LEN / 2 , MAP_LEN - 4 };

void DisplayMap()
{
	for (int i = 0; i < MAP_LEN; i++)
	{
		for (int j = 0; j < MAP_WID; j++)
		{
			switch (g_map[i][j])
			{
			case OBSTACLE:
				WriteStr(i, j, "  ", COLOR_OBSTACLE);
				break;
			case BLOCK:
				WriteStr(i, j, "  ", COLOR_BLOCK);
				break;
			case TREE:
				WriteStr(i, j, "  ", COLOR_TREE);
				break;
			case HEART:
				WriteStr(i, j, "  ", COLOR_HEART);
				break;
			}
		}
	}
}

int SaveMap()
{
	FILE *fp = NULL;
	time_t t;
	struct tm lt;
	char szPath[40] = { 0 };

	system("mkdir map 2> nul");

	time(&t);
	localtime_s(&lt, &t);
	sprintf_s(szPath, 40, "map\\%d_%02d_%02d_%02d_%02d_%02d.map", lt.tm_year + 1900, lt.tm_mon, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);

	fopen_s(&fp, szPath, "wb");
	if (fp == NULL)
	{
		WriteStr(MAP_LEN / 2, WINDOW_WID / 2 - 8, "fopen_s() error\n", COLOR_TEXT);
		return -1;
	}

	fwrite(g_map, 1, sizeof(g_map), fp);
	fclose(fp);

	return 0;
}

int DrawMap()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD inrec;
	DWORD dwRes;
	COORD pos = { 0, 0 };

	char cSelected = BLOCK;
	BOOL bSelected = FALSE;

	/*
		Initialize the map.
	*/
	memset(g_map, LAND, MAP_WID * MAP_LEN);
	memset(g_map, OBSTACLE, MAP_WID);
	for (int i = 1; i < MAP_LEN; i++)
	{
		g_map[i][0] = OBSTACLE;
		g_map[i][MAP_WID - 1] = OBSTACLE;
	}
	memset(&g_map[MAP_LEN - 1], OBSTACLE, MAP_WID);
	g_map[MAP_LEN / 2 - 1][MAP_WID - 2] = BLOCK;
	g_map[MAP_LEN / 2 - 1][MAP_WID - 3] = BLOCK;
	g_map[MAP_LEN / 2 - 1][MAP_WID - 4] = BLOCK;
	g_map[MAP_LEN / 2][MAP_WID - 4] = BLOCK;
	g_map[MAP_LEN / 2 + 1][MAP_WID - 4] = BLOCK;
	g_map[MAP_LEN / 2 + 1][MAP_WID - 3] = BLOCK;
	g_map[MAP_LEN / 2 + 1][MAP_WID - 2] = BLOCK;
	g_map[MAP_LEN / 2][MAP_WID - 3] = HEART;


	DisplayMap();

	WriteStr(MAP_LEN + 3, 4, "block   ", COLOR_BLOCK);
	WriteStr(MAP_LEN + 3, 6, "obstacle", COLOR_OBSTACLE);

	WriteStr(MAP_LEN, 8, "UP/DOWN to select.", COLOR_TEXT);
	WriteStr(MAP_LEN, 10, "RIGHT BUTTON to cancel.", COLOR_TEXT);
	WriteStr(MAP_LEN, 12, "ENTER to save.", COLOR_TEXT);
	
	WriteStr(MAP_LEN + 1, 4, "-->", COLOR_TEXT);


	SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	while (!bSelected)
	{
		ReadConsoleInputW(hIn, &inrec, 1, &dwRes);
		switch (inrec.EventType)
		{
		case MOUSE_EVENT:
			pos = inrec.Event.MouseEvent.dwMousePosition;
			if (FROM_LEFT_1ST_BUTTON_PRESSED == inrec.Event.MouseEvent.dwButtonState &&
				pos.X > 0 &&
				pos.X < MAP_LEN * 2 &&
				pos.Y > 0 &&
				pos.Y < MAP_WID * 2)
			{
				WriteStr(pos.X / 2, pos.Y, "  ", (cSelected == BLOCK) ? COLOR_BLOCK : COLOR_OBSTACLE);
				g_map[pos.X / 2][pos.Y] = cSelected;
			}
			if (RIGHTMOST_BUTTON_PRESSED  == inrec.Event.MouseEvent.dwButtonState &&
				pos.X > 0 &&
				pos.X < MAP_LEN * 2 &&
				pos.Y > 0 &&
				pos.Y < MAP_WID * 2)
			{
				WriteStr(pos.X / 2, pos.Y, "  ", COLOR_LAND);
				g_map[pos.X / 2][pos.Y] = LAND;
			}
			break;
		case KEY_EVENT:
			if (inrec.Event.KeyEvent.bKeyDown)
			{
				switch (inrec.Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_UP:
					if (cSelected == OBSTACLE)
					{
						WriteStr(MAP_LEN + 1, 6, "   ", COLOR_TEXT);
						cSelected = BLOCK;
						WriteStr(MAP_LEN + 1, 4, "-->", COLOR_TEXT);
					}
					break;
				case VK_DOWN:

					if (cSelected == BLOCK)
					{
						WriteStr(MAP_LEN + 1, 4, "   ", COLOR_TEXT);
						cSelected = OBSTACLE;
						WriteStr(MAP_LEN + 1, 6, "-->", COLOR_TEXT);
					}
					break;
				case VK_RETURN:
					bSelected = TRUE;
					break;
				default:
					break;
				}
			}
			break;
		}

	}

	if (SaveMap())
	{
		WriteStr(MAP_LEN / 2, 4, "SaveMap() error", COLOR_TEXT);

	}
	WriteStr(MAP_LEN / 2, 4, "Saved successfully", COLOR_TEXT);

	Sleep(1000);
	SetConsoleTextAttribute(hOut, COLOR_LAND);
	system("cls");

	return 0;
}

int LoadMap(int nMission)
{
	char *szfileNames[MAP_NUM] = { NULL };
	FILE *fp = NULL;
	char szPath[40] = { 0 };
	int nFileNum = 0;

	long hFile = 0;
	struct _finddata_t fileInfo = { 0 };

	int nSelected = 0;
	int nPlayerNum = 1;
	BOOL bSelected = FALSE;

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InRec;
	DWORD dwRes;

	/*
		Single or double mode, a mission should be selected.	
	*/
	if (nMission > 0)
	{
		sprintf_s(szPath, 40, "mission/%d.map",nMission);
		fopen_s(&fp, szPath, "rb");
		if (fp == NULL)
		{
			system("cls");
			WriteStr(MAP_LEN / 2, 5, "fopen_s() error\n", COLOR_TEXT);
			SetConsoleTextAttribute(hOut, COLOR_LAND);
			system("cls");
			return -1;
		}
		fread(g_map, 1, sizeof(g_map), fp);
		fclose(fp);
		fp = NULL;
		return 0;
	}

	strcpy_s(szPath,40,"map/*.map");
	if ((hFile = _findfirst(szPath, &fileInfo)) == -1) {
		WriteStr(MAP_LEN / 2, 6, "No map", COLOR_TEXT);
		Sleep(1000);
		system("cls");
		return -1;
	}

	nFileNum = 0;
	do
	{
		szfileNames[nFileNum] = (char*)malloc(40);
		strcpy_s(szfileNames[nFileNum], 40, fileInfo.name);
		++nFileNum;
	} while (_findnext(hFile, &fileInfo) == 0 && nFileNum < MAP_NUM);


	/*
		Select a map listed.
	*/
	WriteStr(MAP_LEN / 2 - 1, 4, "Choose a map (only 5 maps are displayed):", COLOR_TEXT);
	for (int i = 0; i < nFileNum; i++)
	{
		WriteStr(MAP_LEN / 2, 6 + i * 2, szfileNames[i], COLOR_TEXT);
	}
	WriteStr(MAP_LEN / 2 - 4, 6, "-->", COLOR_TEXT);

	while (!bSelected)
	{
		ReadConsoleInputW(hIn, &InRec, 1, &dwRes);
		if (InRec.EventType == KEY_EVENT && InRec.Event.KeyEvent.bKeyDown)
		{
			switch (InRec.Event.KeyEvent.wVirtualKeyCode)
			{
			case VK_UP:
				if (nSelected > 0)
				{
					WriteStr(MAP_LEN / 2 - 4, 6 + nSelected * 2, "   ", COLOR_TEXT);
					--nSelected;
					WriteStr(MAP_LEN / 2 - 4, 6 + nSelected * 2, "-->", COLOR_TEXT);
				}
				break;
			case VK_DOWN:

				if (nSelected < nFileNum - 1)
				{
					WriteStr(MAP_LEN / 2 - 4, 6 + nSelected * 2, "   ", COLOR_TEXT);
					++nSelected;
					WriteStr(MAP_LEN / 2 - 4, 6 + nSelected * 2, "-->", COLOR_TEXT);
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

	/*
		open file via szPath.
	*/
	strcpy_s(szPath, 40, "map/");
	strcat_s(szPath, 40, szfileNames[nSelected]);
	fopen_s(&fp, szPath, "rb");
	if (fp == NULL)
	{
		SetConsoleTextAttribute(hOut, COLOR_LAND);
		system("cls");
		WriteStr(MAP_LEN / 2 , 5, "fopen_s() error\n", COLOR_TEXT);
		system("cls");
		return -1;
	}

	fread(g_map, 1, sizeof(g_map), fp);
	for (int i = 0; i < nFileNum; i++)
	{
		if (szfileNames[i] != NULL)
		{
			free(szfileNames[i]);
			szfileNames[i] = NULL;
		}
	}

	fclose(fp);
	SetConsoleTextAttribute(hOut, COLOR_LAND);
	system("cls");

	return 0;
}
