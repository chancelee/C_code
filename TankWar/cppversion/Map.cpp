#include "Map.h"


Map::Map()
{
	this->nPlayerBeginPosY = MAP_WID - 4;
	this->nPlayerBeginPosX[0] = MAP_LEN / 2 - 4;
	this->nPlayerBeginPosX[1] = MAP_LEN / 2 + 2;

	this->nEnemyBeginPosY = 1;
	this->nEnemyBeginPosX[0] = 1;
	this->nEnemyBeginPosX[1] = MAP_LEN / 2 ;
	this->nEnemyBeginPosX[2] = MAP_LEN - 4 ;


	memset(aMap, LAND, MAP_WID * MAP_LEN);
	
}

void Map::DisplayMap()
{
	for (int i = 0; i < MAP_LEN; i++)
	{
		for (int j = 0; j < MAP_WID; j++)
		{
			switch (aMap[i][j])
			{
			case OBSTACLE:
				WriteChar(i, j, "  ", COLOR_OBSTACLE);
				break;
			case BLOCK:
				WriteChar(i, j, "  ", COLOR_BLOCK);
				break;
			case TREE:
				WriteChar(i, j, "  ", COLOR_TREE);
				break;
			case HEART:
				WriteChar(i, j, "  ", COLOR_HEART);
				break;
			}
		}
	}
}

int Map::SaveMap()
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
		WriteChar(MAP_LEN / 2, WINDOW_WID / 2 - 8, "fopen_s() error\n", COLOR_TEXT);
		return -1;
	}

	fwrite(aMap, 1, sizeof(aMap), fp);
	fclose(fp);

	return 0;
}

int Map::DrawMap()
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
	memset(aMap, LAND, MAP_WID * MAP_LEN);
	memset(aMap, OBSTACLE, MAP_WID);
	for (int i = 1; i < MAP_LEN; i++)
	{
		aMap[i][0] = OBSTACLE;
		aMap[i][MAP_WID - 1] = OBSTACLE;
	}
	memset(&aMap[MAP_LEN - 1], OBSTACLE, MAP_WID);
	aMap[MAP_LEN / 2 - 1][MAP_WID - 2] = BLOCK;
	aMap[MAP_LEN / 2 - 1][MAP_WID - 3] = BLOCK;
	aMap[MAP_LEN / 2 - 1][MAP_WID - 4] = BLOCK;
	aMap[MAP_LEN / 2][MAP_WID - 4] = BLOCK;
	aMap[MAP_LEN / 2 + 1][MAP_WID - 4] = BLOCK;
	aMap[MAP_LEN / 2 + 1][MAP_WID - 3] = BLOCK;
	aMap[MAP_LEN / 2 + 1][MAP_WID - 2] = BLOCK;
	aMap[MAP_LEN / 2][MAP_WID - 3] = HEART;


	DisplayMap();

	WriteChar(MAP_LEN + 3, 4, "block   ", COLOR_BLOCK);
	WriteChar(MAP_LEN + 3, 6, "obstacle", COLOR_OBSTACLE);

	WriteChar(MAP_LEN, 8, "UP/DOWN to select.", COLOR_TEXT);
	WriteChar(MAP_LEN, 10, "RIGHT BUTTON to cancel.", COLOR_TEXT);
	WriteChar(MAP_LEN, 12, "ENTER to save.", COLOR_TEXT);
	
	WriteChar(MAP_LEN + 1, 4, "-->", COLOR_TEXT);


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
				WriteChar(pos.X / 2, pos.Y, "  ", (cSelected == BLOCK) ? COLOR_BLOCK : COLOR_OBSTACLE);
				aMap[pos.X / 2][pos.Y] = cSelected;
			}
			if (RIGHTMOST_BUTTON_PRESSED  == inrec.Event.MouseEvent.dwButtonState &&
				pos.X > 0 &&
				pos.X < MAP_LEN * 2 &&
				pos.Y > 0 &&
				pos.Y < MAP_WID * 2)
			{
				WriteChar(pos.X / 2, pos.Y, "  ", COLOR_LAND);
				aMap[pos.X / 2][pos.Y] = LAND;
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
						WriteChar(MAP_LEN + 1, 6, "   ", COLOR_TEXT);
						cSelected = BLOCK;
						WriteChar(MAP_LEN + 1, 4, "-->", COLOR_TEXT);
					}
					break;
				case VK_DOWN:

					if (cSelected == BLOCK)
					{
						WriteChar(MAP_LEN + 1, 4, "   ", COLOR_TEXT);
						cSelected = OBSTACLE;
						WriteChar(MAP_LEN + 1, 6, "-->", COLOR_TEXT);
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
		WriteChar(MAP_LEN / 2, 4, "SaveMap() error", COLOR_TEXT);

	}
	WriteChar(MAP_LEN / 2, 4, "Saved successfully", COLOR_TEXT);

	Sleep(1000);
	SetConsoleTextAttribute(hOut, COLOR_LAND);
	system("cls");

	return 0;
}

int Map::LoadMap(int nMission)
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
			WriteChar(MAP_LEN / 2, 5, "fopen_s() error\n", COLOR_TEXT);
			SetConsoleTextAttribute(hOut, COLOR_LAND);
			system("cls");
			return -1;
		}
		fread(aMap, 1, sizeof(aMap), fp);
		fclose(fp);
		fp = NULL;
		return 0;
	}

	strcpy_s(szPath,40,"map/*.map");
	if ((hFile = _findfirst(szPath, &fileInfo)) == -1) {
		WriteChar(MAP_LEN / 2, 6, "No map", COLOR_TEXT);
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
	WriteChar(MAP_LEN / 2 - 1, 4, "Choose a map (only 5 maps are displayed):", COLOR_TEXT);
	for (int i = 0; i < nFileNum; i++)
	{
		WriteChar(MAP_LEN / 2, 6 + i * 2, szfileNames[i], COLOR_TEXT);
	}
	WriteChar(MAP_LEN / 2 - 4, 6, "-->", COLOR_TEXT);

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
					WriteChar(MAP_LEN / 2 - 4, 6 + nSelected * 2, "   ", COLOR_TEXT);
					--nSelected;
					WriteChar(MAP_LEN / 2 - 4, 6 + nSelected * 2, "-->", COLOR_TEXT);
				}
				break;
			case VK_DOWN:

				if (nSelected < nFileNum - 1)
				{
					WriteChar(MAP_LEN / 2 - 4, 6 + nSelected * 2, "   ", COLOR_TEXT);
					++nSelected;
					WriteChar(MAP_LEN / 2 - 4, 6 + nSelected * 2, "-->", COLOR_TEXT);
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
		WriteChar(MAP_LEN / 2 , 5, "fopen_s() error\n", COLOR_TEXT);
		system("cls");
		return -1;
	}

	fread(aMap, 1, sizeof(aMap), fp);
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
