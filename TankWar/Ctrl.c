#include "Ctrl.h"

#pragma comment(lib,"winmm.lib")

void SetPlayerNum(int *pPlayerNum)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	BOOL bSelected = FALSE;
	INPUT_RECORD keyrec;
	DWORD res = 0;
	int x = WINDOW_LEN / 6, y = WINDOW_WID / 2 - 8;

	
	*pPlayerNum = 1;
	
	WriteStr(x, y, "1 player", COLOR_TEXT_SELECTED);
	WriteStr(x, y + 2, "2 players", COLOR_TEXT);
	while (!bSelected)
	{
		ReadConsoleInput(hIn, &keyrec, 1, &res);
		if (keyrec.EventType == KEY_EVENT
			&& keyrec.Event.KeyEvent.bKeyDown)
		{
			switch (keyrec.Event.KeyEvent.wVirtualKeyCode)
			{
			case VK_UP:
				if (*pPlayerNum == 2)
				{
					WriteStr(x, y + 2, "2 players", COLOR_TEXT);
					--(*pPlayerNum);
					WriteStr(x, y, "1 player ", COLOR_TEXT_SELECTED);

				}
				break;
			case VK_DOWN:

				if (*pPlayerNum == 1)
				{
					WriteStr(x, y, "1 player ", COLOR_TEXT);
					++(*pPlayerNum);
					WriteStr(x, y + 2, "2 players", COLOR_TEXT_SELECTED);
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
	SetConsoleTextAttribute(hOut, COLOR_LAND);
	system("cls");
}

void InitPlayersAndEnemies(PTank pPlayer, int nPlayerNum, PTank pEnemy)
{
	
	InitTank(&pPlayer[0], TRUE, g_playerBeginPosX[0], g_playerBeginPosY, UP, COLOR_PLAYER1);
	if (nPlayerNum == 2)
	{
		InitTank(&pPlayer[1], TRUE, g_playerBeginPosX[1], g_playerBeginPosY, UP, COLOR_PLAYER2);

	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		InitTank(&pEnemy[i], FALSE, g_enemyBeginPosX[i % 3], g_enemyBeginPosY, DOWN, COLOR_ENEMY);
	}
}

void StartGame(int nMission,PTank pPlayer,int nPlayerNum,PTank pEnemy)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	BYTE cChoice = 0;
	BOOL bWin = FALSE;



	clock_t tank_time_start = clock();
	clock_t bullet_time_start = clock();
	clock_t tank_time_end, bullet_time_end;
	clock_t enemy_time_start = clock(), enemy_time_end;
	int nEnemyNum = 0;

	

	/*
	Load this mission's map.
	*/
	if (nMission >= 0)
	{
		/*
		All mission completed.
		*/
		if (nMission == 4)
		{
			WriteStr(MAP_LEN / 2, 6, "Congratulation !", COLOR_TEXT);
			_getch();
			return;
		}

		if (LoadMap(nMission)) return;
	}
	DisplayMap();

	
	//
	for (int i = 0; i < nPlayerNum; i++)
	{
		MarkTank(&pPlayer[i],i);
		DisplayTank(&pPlayer[i]);
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (pEnemy[i].m_nHP) ++nEnemyNum;
	}
	
	PlaySoundA("sound\\bgm.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);

	WriteStr(MAP_LEN   , 4, "player 1 ", COLOR_TEXT);
	WriteStr(MAP_LEN   , 5, "     w/s/a/d/f", COLOR_TEXT);
	WriteStr(MAP_LEN   , 6, " HP ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 6, pPlayer[0].m_nHP, COLOR_TEXT);
	WriteStr(MAP_LEN   , 7, " score: ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 7, pPlayer[0].m_nKill, COLOR_TEXT);

	WriteStr(MAP_LEN   , 9, "player 2 ", COLOR_TEXT);
	WriteStr(MAP_LEN   , 10, "     i/k/j/l/h", COLOR_TEXT);
	WriteStr(MAP_LEN   , 11, " HP   : ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 11, pPlayer[1].m_nHP, COLOR_TEXT);
	WriteStr(MAP_LEN   , 12, " score: ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 12, pPlayer[1].m_nKill, COLOR_TEXT);
	
	WriteStr(MAP_LEN + 4, 14, "B to save", COLOR_TEXT);

	srand((unsigned int)time(0));
	/*
		Begin the main loop.
	*/
	while ( (pPlayer[0].m_nHP || pPlayer[1].m_nHP)
		&& (g_map[MAP_LEN / 2][MAP_WID - 3] == HEART)
		&& (pPlayer[0].m_nKill + pPlayer[1].m_nKill) < ENEMY_NUM)
	{
		if (_kbhit())
		{
			cChoice = _getch();
			switch (cChoice)
			{
			case 'w':
				MoveInMap(&pPlayer[0], UP);
				break;
			case 's':
				MoveInMap(&pPlayer[0], DOWN);
				break;
			case 'a':
				MoveInMap(&pPlayer[0], LEFT);
				break;
			case 'd':
				MoveInMap(&pPlayer[0], RIGHT);
				break;
			case 'f':
				Fire(&pPlayer[0]);
				break;
			case 'i':
				MoveInMap(&pPlayer[1], UP);
				break;
			case 'k':
				MoveInMap(&pPlayer[1], DOWN);
				break;
			case 'j':
				MoveInMap(&pPlayer[1], LEFT);
				break;
			case 'l':
				MoveInMap(&pPlayer[1], RIGHT);
				break;
			case 'h':
				Fire(&pPlayer[1]);
				break;
			case 'b':
				if (!SaveBk(nMission,pPlayer, pEnemy))
				{
					WriteStr(MAP_LEN, 16, "Backup saved", COLOR_TEXT);
					Sleep(1500);
					WriteStr(MAP_LEN, 16, "            ", COLOR_LAND);
				}
			default:
				break;
			}

		}

		// Display the tanks.
		tank_time_end = clock();
		if (tank_time_end - tank_time_start > TANK_SPEED)
		{
			tank_time_start = tank_time_end;

			for (int i = 0; i < nPlayerNum; i++)
			{
				if (pPlayer[i].m_nHP)
				{
					ClsTank(&pPlayer[i]);
					UnmarkTank(&pPlayer[i]);
					pPlayer[i].m_oldPosX = pPlayer[i].m_posX;
					pPlayer[i].m_oldPosY = pPlayer[i].m_posY;
					MarkTank(&pPlayer[i],i);
					DisplayTank(&pPlayer[i]);
				}
			}


			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (pEnemy[i].m_nHP)
				{
					(rand() % 3) ? Fire(&pEnemy[i]) : 0;
					if (!MoveInMap(&pEnemy[i], pEnemy[i].m_nDirection))
					{
						switch (pEnemy[i].m_nDirection)
						{
						case UP:
							pEnemy[i].m_nDirection = (rand() % 2) ? DOWN : ((rand() % 2) ? LEFT : RIGHT);
							break;
						case DOWN:
							pEnemy[i].m_nDirection = (rand() % 2) ? LEFT : ((rand() % 2) ? RIGHT : UP);
							break;
						case LEFT:
							pEnemy[i].m_nDirection = (rand() % 2) ? DOWN : ((rand() % 2) ? RIGHT : UP);
							break;
						case RIGHT:
							pEnemy[i].m_nDirection = (rand() % 2) ? DOWN : ((rand() % 2) ? LEFT : UP);
							break;
						}
					}

					ClsTank(&pEnemy[i]);
					UnmarkTank(&pEnemy[i]);
					pEnemy[i].m_oldPosX = pEnemy[i].m_posX;
					pEnemy[i].m_oldPosY = pEnemy[i].m_posY;
					MarkTank(&pEnemy[i],i);
					DisplayTank(&pEnemy[i]);
				}
			}

		}

		// Display the bullets.
		bullet_time_end = clock();
		if (bullet_time_end - bullet_time_start > BULLET_SPEED)
		{
			bullet_time_start = bullet_time_end;
			for (int i = 0; i < nPlayerNum; i++)
			{
				BulletCollisionCheck(pPlayer, nPlayerNum, pEnemy);
				BulletMoveAndDisplay(pPlayer, nPlayerNum, pEnemy, ENEMY_NUM);
			}
		}

		// The enemies should show one by one every 3 seconds.
		if (nEnemyNum < ENEMY_NUM)
		{
			enemy_time_end = clock();
			if ((enemy_time_end - enemy_time_start) % 3000 == 0) {
				pEnemy[nEnemyNum].m_nHP = 1;
				MarkTank(&pEnemy[nEnemyNum], nEnemyNum);
				DisplayTank(&pEnemy[nEnemyNum]);
				++nEnemyNum;
			}

		}

	}

	if ( (pPlayer[0].m_nKill + pPlayer[1].m_nKill) == ENEMY_NUM)
	{
		WriteStr(MAP_LEN / 2, MAP_WID / 2, "MISSION COMPLETED!", COLOR_TEXT);

		_getch();

		SetConsoleTextAttribute(hOut,COLOR_LAND);
		system("cls");
		if (nMission > 0)
		{
			InitPlayersAndEnemies(pPlayer, nPlayerNum, pEnemy);
			StartGame(++nMission,pPlayer, nPlayerNum, pEnemy);
		}
		else
		{
			return;
		}
	}
	else
	{
		WriteStr(MAP_LEN / 2, MAP_WID / 2, "GAGE OVER", COLOR_TEXT);
		WriteStr(MAP_LEN / 2, MAP_WID / 2 + 1, "Press any key to return.", COLOR_TEXT);
		_getch();
		SetConsoleTextAttribute(hOut, COLOR_LAND);
		system("cls");
		return;
	}
	
}


BOOL MoveInMap(PTank pTank, BYTE dir)
{
	switch (dir)
	{
	case UP:
		if (pTank->m_nDirection == UP)
		{
			if (g_map[pTank->m_posX][pTank->m_posY - 1] == LAND
				&& g_map[pTank->m_posX + 1][pTank->m_posY - 1] == LAND
				&& g_map[pTank->m_posX + 2][pTank->m_posY - 1] == LAND
				)
			{
				--pTank->m_posY;
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			pTank->m_nDirection = UP;
			return TRUE;
		}
		break;
	case DOWN:
		if (pTank->m_nDirection == DOWN)
		{
			if (g_map[pTank->m_posX][pTank->m_posY + 3] == LAND
				&& g_map[pTank->m_posX + 1][pTank->m_posY + 3] == LAND
				&& g_map[pTank->m_posX + 2][pTank->m_posY + 3] == LAND
				)
			{
				++pTank->m_posY;
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			pTank->m_nDirection = DOWN;
			return TRUE;
		}
		break;
	case LEFT:
		if (pTank->m_nDirection == LEFT)
		{
			if (g_map[pTank->m_posX - 1][pTank->m_posY] == LAND
				&& g_map[pTank->m_posX - 1][pTank->m_posY + 1] == LAND
				&& g_map[pTank->m_posX - 1][pTank->m_posY + 2] == LAND
				)
			{
				--pTank->m_posX;
				return TRUE;
			}
			return FALSE;

		}
		else
		{
			pTank->m_nDirection = LEFT;
			return TRUE;
		}
		break;
	case RIGHT:
		if (pTank->m_nDirection == RIGHT)
		{
			if (g_map[pTank->m_posX + 3][pTank->m_posY] == LAND
				&& g_map[pTank->m_posX + 3][pTank->m_posY + 1] == LAND
				&& g_map[pTank->m_posX + 3][pTank->m_posY + 2] == LAND
				)
			{
				++pTank->m_posX;
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			pTank->m_nDirection = RIGHT;
			return TRUE;
		}
		break;
	}

}

void BulletCollisionCheck(PTank pPlayer, int nPlayerNum, PTank pEnemy)
{

	/*
		For each player's bullets
		index: player - i;  bullets - j; enemy - k.
	*/
	for (int i = 0; i < nPlayerNum; i++)
	{
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pPlayer[i].m_bullets[j].used)
			{
				int k = 0;
				switch (g_map[pPlayer[i].m_bullets[j].m_posX][pPlayer[i].m_bullets[j].m_posY] & 0xF0)
				{
				case PLAYER:
					//player shoot player
					pPlayer[i].m_bullets[j].used = 0;

					break;
				case ENEMY:
					/*
						player shoot enemy
					*/
					PlaySoundA("sound\\pwn.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
					pPlayer[i].m_bullets[j].used = 0;
					


					k = g_map[pPlayer[i].m_bullets[j].m_posX][pPlayer[i].m_bullets[j].m_posY] & 0x0F;
					--pEnemy[k].m_nHP;
					WriteInt(MAP_LEN + 4, 7 + 5 * i, ++pPlayer[i].m_nKill, COLOR_TEXT);
					UnmarkTank(&pEnemy[k]);
					ClsTank(&pEnemy[k]);
					break;
				case HEART:
					g_map[pPlayer[i].m_bullets[j].m_posX][pPlayer[i].m_bullets[j].m_posY] = LAND;
					break;
				case OBSTACLE:
					pPlayer[i].m_bullets[j].used = 0;
					WriteStr(pPlayer[i].m_bullets[j].m_posX, pPlayer[i].m_bullets[j].m_posY, "  ", COLOR_OBSTACLE);
					break;
				case BLOCK:
					g_map[pPlayer[i].m_bullets[j].m_posX][pPlayer[i].m_bullets[j].m_posY] = LAND;
					pPlayer[i].m_bullets[j].used = 0;
					WriteStr(pPlayer[i].m_bullets[j].m_posX, pPlayer[i].m_bullets[j].m_posY, "  ", COLOR_LAND);
					break;

				default:
					break;
				}
			}
		}
	}

	/*
	For each enemy's bullets
	index: enemy - i;  bullets - j;  player - k.
	*/
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pEnemy[i].m_bullets[j].used)
			{
				int k = 0;
				switch (g_map[pEnemy[i].m_bullets[j].m_posX][pEnemy[i].m_bullets[j].m_posY] & 0xF0)
				{
				case PLAYER:
					/*
						enemy shoot player
					*/
					PlaySoundA("sound\\pwn.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
					pEnemy[i].m_bullets[j].used = 0;
					
					k = g_map[pEnemy[i].m_bullets[j].m_posX][pEnemy[i].m_bullets[j].m_posY] & 0x0F;
					--(pPlayer[k].m_nHP);
					WriteInt(MAP_LEN + 4, 6 + 5 * k, pPlayer[k].m_nHP, COLOR_TEXT);

					UnmarkTank(&pPlayer[k]);
					ClsTank(&pPlayer[k]);
					pPlayer[k].m_posX = (k ? g_playerBeginPosX[1] : g_playerBeginPosX[0]);
					pPlayer[k].m_posY = g_playerBeginPosY;
					break;
				case ENEMY:
					//enemy shoot enemy
					pEnemy[i].m_bullets[j].used = 0;
					break;
					
				case HEART:
					/*
						Game over.
					*/
					g_map[pEnemy[i].m_bullets[j].m_posX][pEnemy[i].m_bullets[j].m_posY] = LAND;
					break;
				case OBSTACLE:
					pEnemy[i].m_bullets[j].used = 0;
					WriteStr(pEnemy[i].m_bullets[j].m_posX, pEnemy[i].m_bullets[j].m_posY, "  ", COLOR_OBSTACLE);
					break;
				case BLOCK:
					g_map[pEnemy[i].m_bullets[j].m_posX][pEnemy[i].m_bullets[j].m_posY] = LAND;
					pEnemy[i].m_bullets[j].used = 0;
					WriteStr(pEnemy[i].m_bullets[j].m_posX, pEnemy[i].m_bullets[j].m_posY, "  ", COLOR_LAND);
					break;
				default:
					break;
				}
			}
		}
	}

	

}

void MarkTank(PTank pTank,int nIndex)
{

	for (int y = pTank->m_posY; y < pTank->m_posY + 3; y++)
	{
		for (int x = pTank->m_posX; x < pTank->m_posX + 3; x++)
		{
			g_map[x][y] = (pTank->m_bIsPlayer ? PLAYER : ENEMY);
			g_map[x][y] |= nIndex;
		}
	}

}

void UnmarkTank(PTank pTank)
{
	for (int y = pTank->m_oldPosY; y < pTank->m_oldPosY + 3; y++)
	{
		for (int x = pTank->m_oldPosX; x < pTank->m_oldPosX + 3; x++)
		{
			g_map[x][y] = LAND;
		}
	}
}

int SaveBk(int nMission,PTank pPlayer,PTank pEnemy)
{
	time_t t;
	struct tm lt;
	char szPath[40] = { 0 };
	FILE *fp = NULL;


	time(&t);
	localtime_s(&lt, &t);
	sprintf_s(szPath, 40, "backup\\%d_%02d_%02d_%02d_%02d_%02d.bk", lt.tm_year + 1900, lt.tm_mon, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);

	system("mkdir backup 2>nul");

	fopen_s(&fp, szPath, "wb");
	if (fp == NULL)
	{
		WriteStr(MAP_LEN / 2, WINDOW_WID / 2 - 8, "fopen_s() error\n", COLOR_TEXT);
		return -1;
	}

	fwrite(g_map, 1, sizeof(g_map), fp);
	fwrite(&nMission, sizeof(nMission), 1, fp);
	fwrite(pPlayer, sizeof(Tank), 2, fp);
	fwrite(pEnemy, sizeof(Tank), ENEMY_NUM, fp);


	fclose(fp);
	return 0;
}

int LoadBk(int *pnMission, PTank pPlayer, PTank pEnemy)
{
	char *szfileNames[MAP_NUM] = { NULL };
	FILE *fp = NULL;
	char szPath[40] = { 0 };
	int nFileNum = 0;

	long hFile = 0;
	struct _finddata_t fileInfo = { 0 };

	int nSelected = 0;
	BOOL bSelected = FALSE;

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InRec;
	DWORD dwRes;

	strcpy_s(szPath, 40, "backup/*.bk");
	if ((hFile = _findfirst(szPath, &fileInfo)) == -1) {
		WriteStr(30, 10, "No map", COLOR_TEXT);
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

	WriteStr(MAP_LEN / 2 - 1, 4, "Choose a backup (only 5 backups are displayed):", COLOR_TEXT);
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
	strcpy_s(szPath, 40, "backup/");
	strcat_s(szPath, 40, szfileNames[nSelected]);
	fopen_s(&fp, szPath, "rb");
	if (fp == NULL)
	{
		SetConsoleTextAttribute(hOut, COLOR_LAND);
		system("cls");
		WriteStr(MAP_LEN / 2, 5, "fopen_s() error\n", COLOR_TEXT);
		system("cls");
		return -1;
	}

	fread(g_map, 1, sizeof(g_map), fp);
	fread(pnMission, sizeof(*pnMission), 1, fp);
	fread(pPlayer, sizeof(Tank), 2, fp);
	fread(pEnemy, sizeof(Tank), ENEMY_NUM, fp);


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

