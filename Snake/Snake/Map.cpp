#include "Map.h"

/*
	Global map stored in 2-d array variable.
*/
BYTE g_map[MAP_LEN][MAP_WID] = { LAND };

/*
	Set random obstacles.
*/
void InitMap()
{
	int nX = 0, nY = 0;
	int nTemp = 10;

	memset(g_map, LAND, MAP_WID * MAP_LEN);

	memset(g_map, OBSTACLE, MAP_WID);
	for (int i = 1; i < MAP_LEN; i++)
	{
		g_map[i][0] = OBSTACLE;
		g_map[i][MAP_WID - 1] = OBSTACLE;
	}
	memset(&g_map[MAP_LEN - 1], OBSTACLE, MAP_WID);

	srand((unsigned int)time((time_t)0));
	while (nTemp)
	{
		nX = rand() % (MAP_LEN - 3) + 1;
		nY = rand() % (MAP_WID - 3) + 1;
		if (nX != 10)
		{
			g_map[nX][nY] = OBSTACLE;
			nTemp--;
			continue;
		}
	}
	

}

/*
	Show the global map on the screen.
*/
void GenMap()
{
	

	for (int i = 0; i < MAP_LEN; i++)
	{
		for (int j = 0; j < MAP_WID; j++)
		{
			switch (g_map[i][j])
			{
			case OBSTACLE:
				WriteChar(i, j, "  ",COLOR_OBSTACLE);
				break;
			case FOOD:
				
				WriteChar(i, j, "  ", COLOR_FOOD);
				break;
			}
		}
	}
}


/*
	Design a map by the player.
*/
int DrawMap()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD mouseRec;
	DWORD dwTemp;
	COORD pos = { 0, 0 };

	time_t t;
	struct tm lt;

	FILE *fp = nullptr;
	char szPath[40] = { 0 };

	memset(g_map, LAND, MAP_WID * MAP_LEN);
	memset(g_map, OBSTACLE, MAP_WID);
	for (int i = 1; i < MAP_LEN; i++)
	{
		g_map[i][0] = OBSTACLE;
		g_map[i][MAP_WID - 1] = OBSTACLE;
	}
	memset(&g_map[MAP_LEN - 1], OBSTACLE, MAP_WID);

	GenMap();

	WriteChar(MAP_LEN,4,"double click to save", COLOR_TEXT);
	SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	while (1) 
	{
		ReadConsoleInputW(hIn, &mouseRec, 1, &dwTemp);
		if (mouseRec.EventType == MOUSE_EVENT)
		{
			pos = mouseRec.Event.MouseEvent.dwMousePosition;
			if (DOUBLE_CLICK == mouseRec.Event.MouseEvent.dwEventFlags)
			{
				break;
			}
			if (FROM_LEFT_1ST_BUTTON_PRESSED == mouseRec.Event.MouseEvent.dwButtonState &&
				pos.X > 0 &&
				pos.X < MAP_LEN * 2 &&
				pos.Y > 0 &&
				pos.Y < MAP_WID * 2)
			{
				WriteChar(pos.X / 2,pos.Y," ",COLOR_OBSTACLE);
				g_map[pos.X / 2][pos.Y] = OBSTACLE;
			}
		}
	}

	system("mkdir map 2> nul");

	time(&t);
	localtime_s(&lt, &t);
	sprintf_s(szPath, 40, "map\\%d%02d%02d%02d%02d%02d.map", lt.tm_year + 1900, lt.tm_mon, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);

	fopen_s(&fp,szPath,"wb");
	if (fp == NULL)
	{
		WriteChar(30, 10, "fopen_s() error\n", COLOR_TEXT);
		return -1;
	}

	fwrite(g_map,1,sizeof(g_map),fp);
	fclose(fp);

	WriteChar(MAP_LEN, 4, "Saved successfully...", COLOR_TEXT);
	Sleep(1000);
	system("cls");

	
	return 0;
}

/*
	Load the map designed by the player, which are in the map directory.
*/
int LoadMap()
{
	vector<char*> vpBk;
	string pathName;
	long hFile = 0;
	struct _finddata_t fileInfo;

	FILE *fp = nullptr;
	char szPath[40] = "map\\";
	int nOp = -1;

	HANDLE hIn = nullptr;
	INPUT_RECORD InRec;
	COORD pos = { 0, 0 };
	DWORD dwTemp;

	if ((hFile = _findfirst(pathName.assign(szPath).append("\\*.map").c_str(), &fileInfo)) == -1) {
		WriteChar(30, 10, "No map", COLOR_TEXT);
		Sleep(1000);
		system("cls");
		return -1;
	}
	do
	{
		char *buf = (char*)malloc(20);
		strcpy_s(buf, 20, fileInfo.name);
		vpBk.push_back(buf);
	} while (_findnext(hFile, &fileInfo) == 0);

	WriteChar(30, 2, "Choose a map:", COLOR_TEXT);
	for (int i = 0; i < vpBk.size(); i++)
	{
		WriteInt(30, 4 + i * 2 + 1, i + 1);
		WriteChar(32, 4 + i * 2 + 1, vpBk[i], COLOR_TEXT);
	}


	hIn = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	while (nOp == -1)
	{

		ReadConsoleInputW(hIn, &InRec, 1, &dwTemp);
		if (InRec.EventType == MOUSE_EVENT)
		{
			pos = InRec.Event.MouseEvent.dwMousePosition;
			
			if (FROM_LEFT_1ST_BUTTON_PRESSED == InRec.Event.MouseEvent.dwButtonState)
			{
				
				for (int i = 0; i < vpBk.size(); i++)
				{
					if (pos.X >= 60 && pos.X <= 80 && pos.Y == (4 + i * 2 + 1))
					{
						nOp = i;
						WriteInt(0, 7,nOp);
						break;
					}
				}
			}
		}
	}
	strcat_s(szPath, 40, vpBk[nOp]);
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
		WriteChar(30, 10, "fopen_s() error\n", COLOR_TEXT);
		return -1;
	}

	fread(g_map, 1, sizeof(g_map), fp);

	fclose(fp);

	system("cls");
	return 0;
}


