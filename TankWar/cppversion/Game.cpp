#include "Game.h"

#pragma comment(lib,"winmm.lib")

Game::Game()
{
	m_pPlayer[0] = new Tank(TRUE, m_map.nPlayerBeginPosX[0], m_map.nPlayerBeginPosY, UP, COLOR_PLAYER1);
	m_pPlayer[1] = new Tank(TRUE, m_map.nPlayerBeginPosX[1], m_map.nPlayerBeginPosY, UP, COLOR_PLAYER2);
	if (m_nPlayerNum == 1)
	{
		memset(m_pPlayer[1], 0, sizeof(Tank));
	}


	for (int i = 0; i < ENEMY_NUM; i++)
	{
		m_pEnemy[i] = new Tank(FALSE, m_map.nEnemyBeginPosX[i % 3], m_map.nEnemyBeginPosY, DOWN, COLOR_ENEMY);
	}

	m_nPlayerNum = 1;
	m_nEnemyNum = 0;
	m_nMission = 1;
}

Game::~Game()
{
	delete[] m_pPlayer;
	m_pPlayer[0] = nullptr;
	m_pPlayer[1] = nullptr;

	delete[] m_pEnemy;
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		m_pEnemy[i] = nullptr;
	}

}

void Game::SetPlayerNum()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	BOOL bSelected = FALSE;
	INPUT_RECORD keyrec;
	DWORD res = 0;
	int x = WINDOW_LEN / 6, y = WINDOW_WID / 2 - 8;


	this->m_nPlayerNum = 1;

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
				if (this->m_nPlayerNum == 2)
				{
					WriteStr(x, y + 2, "2 players", COLOR_TEXT);
					--(this->m_nPlayerNum);
					WriteStr(x, y, "1 player ", COLOR_TEXT_SELECTED);

				}
				break;
			case VK_DOWN:

				if (this->m_nPlayerNum == 1)
				{
					WriteStr(x, y, "1 player ", COLOR_TEXT);
					++(this->m_nPlayerNum);
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

void Game::InitPlayerAndEnemy()
{
	for (int i = 0; i < 2; ++i)
	{
		m_pPlayer[i]->InitTank(m_map.nPlayerBeginPosX[0], m_map.nPlayerBeginPosY, UP);
	}
	for (int i = 0; i < ENEMY_NUM; ++i)
	{
		m_pEnemy[i]->InitTank(m_map.nEnemyBeginPosX[i % 3], m_map.nEnemyBeginPosY, DOWN);
	}
	m_nEnemyNum = 0;
}


void Game::StartGame(int nMission)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	BYTE cChoice = 0;
	BOOL bWin = FALSE;

	clock_t tank_time_start = clock();
	clock_t bullet_time_start = clock();
	clock_t tank_time_end, bullet_time_end;
	clock_t enemy_time_start = clock(), enemy_time_end;


	if (nMission >= 0)
	{
		m_nMission = nMission;

		/*
			All mission completed.
		*/
		if (m_nMission == 4)
		{
			WriteStr(MAP_LEN / 2, 6, "Congratulation !", COLOR_TEXT);
			_getch();
			return;
		}

		/*
		Load this mission's map.
		*/
		if (m_map.LoadMap(m_nMission)) return;

	}

	m_map.DisplayMap();

	
	//
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		MarkTank(m_pPlayer[i],i);
		m_pPlayer[i]->DisplayTank();
	}
	for (int i = 0; i < ENEMY_NUM; ++i)
	{
		if (m_pEnemy[i]->m_nHP) ++m_nEnemyNum;
	}
	
	PlaySoundA("sound\\bgm.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);

	WriteStr(MAP_LEN   , 4, "player 1 ", COLOR_TEXT);
	WriteStr(MAP_LEN   , 5, "     w/s/a/d/f", COLOR_TEXT);
	WriteStr(MAP_LEN   , 6, " HP ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 6, m_pPlayer[0]->m_nHP, COLOR_TEXT);
	WriteStr(MAP_LEN   , 7, " score: ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 7, m_pPlayer[0]->m_nKill, COLOR_TEXT);

	WriteStr(MAP_LEN   , 9, "player 2 ", COLOR_TEXT);
	WriteStr(MAP_LEN   , 10, "     i/k/j/l/h", COLOR_TEXT);
	WriteStr(MAP_LEN   , 11, " HP   : ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 11, m_pPlayer[1]->m_nHP, COLOR_TEXT);
	WriteStr(MAP_LEN   , 12, " score: ", COLOR_TEXT);
	WriteInt(MAP_LEN + 4, 12, m_pPlayer[1]->m_nKill, COLOR_TEXT);
	
	WriteStr(MAP_LEN + 4, 14, "B to save", COLOR_TEXT);

	srand((unsigned int)time(0));
	/*
		Begin the main loop.
	*/
	while ( (m_pPlayer[0]->m_nHP || m_pPlayer[1]->m_nHP)
		&& (m_map.aMap[HEART_X][HEART_Y] == HEART)
		&& (m_pPlayer[0]->m_nKill + m_pPlayer[1]->m_nKill) < ENEMY_NUM)
	{
		if (_kbhit())
		{
			cChoice = _getch();
			switch (cChoice)
			{
			case 'w':
				MoveInMap(m_pPlayer[0], UP);
				break;
			case 's':
				MoveInMap(m_pPlayer[0], DOWN);
				break;
			case 'a':
				MoveInMap(m_pPlayer[0], LEFT);
				break;
			case 'd':
				MoveInMap(m_pPlayer[0], RIGHT);
				break;
			case 'f':
				m_pPlayer[0]->Fire();
				break;
			case 'i':
				MoveInMap(m_pPlayer[1], UP);
				break;
			case 'k':
				MoveInMap(m_pPlayer[1], DOWN);
				break;
			case 'j':
				MoveInMap(m_pPlayer[1], LEFT);
				break;
			case 'l':
				MoveInMap(m_pPlayer[1], RIGHT);
				break;
			case 'h':
				m_pPlayer[1]->Fire();
				break;
			case 'b':
				if (!SaveBk())
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

			for (int i = 0; i < m_nPlayerNum; i++)
			{
				if (m_pPlayer[i]->m_nHP)
				{
					m_pPlayer[i]->ClsTank();
					UnmarkTank(m_pPlayer[i]);
					m_pPlayer[i]->m_oldPosX = m_pPlayer[i]->m_posX;
					m_pPlayer[i]->m_oldPosY = m_pPlayer[i]->m_posY;
					MarkTank(m_pPlayer[i],i);
					m_pPlayer[i]->DisplayTank();
				}
			}


			for (int i = 0; i < ENEMY_NUM; i++)
			{
				if (m_pEnemy[i]->m_nHP)
				{
					(rand() % 3) ? m_pEnemy[i]->Fire() : 0;
					if (!MoveInMap(m_pEnemy[i], m_pEnemy[i]->m_nDirection))
					{
						switch (m_pEnemy[i]->m_nDirection)
						{
						case UP:
							m_pEnemy[i]->m_nDirection = (rand() % 2) ? DOWN : ((rand() % 2) ? LEFT : RIGHT);
							break;
						case DOWN:
							m_pEnemy[i]->m_nDirection = (rand() % 2) ? LEFT : ((rand() % 2) ? RIGHT : UP);
							break;
						case LEFT:
							m_pEnemy[i]->m_nDirection = (rand() % 2) ? DOWN : ((rand() % 2) ? RIGHT : UP);
							break;
						case RIGHT:
							m_pEnemy[i]->m_nDirection = (rand() % 2) ? DOWN : ((rand() % 2) ? LEFT : UP);
							break;
						}
					}

					m_pEnemy[i]->ClsTank();
					UnmarkTank(m_pEnemy[i]);
					m_pEnemy[i]->m_oldPosX = m_pEnemy[i]->m_posX;
					m_pEnemy[i]->m_oldPosY = m_pEnemy[i]->m_posY;
					MarkTank(m_pEnemy[i],i);
					m_pEnemy[i]->DisplayTank();
				}
			}

		}

		// Display the bullets.
		bullet_time_end = clock();
		if (bullet_time_end - bullet_time_start > BULLET_SPEED)
		{
			bullet_time_start = bullet_time_end;

			BulletCollisionCheck();

			for (int i = 0; i < m_nPlayerNum; i++)
			{
				m_pPlayer[i]->BulletMoveAndDisplay();
				
			}
			for (int i = 0; i < ENEMY_NUM; i++)
			{
				m_pEnemy[i]->BulletMoveAndDisplay();
			}

			
		}

		// The enemies should show one by one every 3 seconds.
		if (m_nEnemyNum < ENEMY_NUM)
		{
			enemy_time_end = clock();
			if ((enemy_time_end - enemy_time_start) % 3000 == 0) {
				m_pEnemy[m_nEnemyNum]->m_nHP = 1;
				MarkTank(m_pEnemy[m_nEnemyNum], m_nEnemyNum);
				m_pEnemy[m_nEnemyNum]->DisplayTank();
				++m_nEnemyNum;
			}

		}

	}

	m_nEnemyNum = 0;

	if ( (m_pPlayer[0]->m_nKill + m_pPlayer[1]->m_nKill) == ENEMY_NUM)
	{
		WriteStr(MAP_LEN / 2, MAP_WID / 2, "MISSION COMPLETED!", COLOR_TEXT);

		_getch();

		SetConsoleTextAttribute(hOut,COLOR_LAND);
		system("cls");
		if (m_nMission > 0)
		{
			InitPlayerAndEnemy();
			StartGame(m_nMission + 1);
		}
		else
		{
			SetConsoleTextAttribute(hOut, COLOR_LAND);
			system("cls");
			return;
		}
	}
	else
	{
		m_map.aMap[HEART_X][HEART_Y] = HEART;

		WriteStr(MAP_LEN / 2, MAP_WID / 2, "GAGE OVER", COLOR_TEXT);
		WriteStr(MAP_LEN / 2, MAP_WID / 2 + 1, "Press any key to return.", COLOR_TEXT);
		_getch();
		SetConsoleTextAttribute(hOut, COLOR_LAND);
		system("cls");
		return;
	}
	
}


BOOL Game::MoveInMap(PTank pTank, BYTE dir)
{
	switch (dir)
	{
	case UP:
		if (pTank->m_nDirection == UP)
		{
			if (m_map.aMap[pTank->m_posX][pTank->m_posY - 1] == LAND
				&& m_map.aMap[pTank->m_posX + 1][pTank->m_posY - 1] == LAND
				&& m_map.aMap[pTank->m_posX + 2][pTank->m_posY - 1] == LAND
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
			if (m_map.aMap[pTank->m_posX][pTank->m_posY + 3] == LAND
				&& m_map.aMap[pTank->m_posX + 1][pTank->m_posY + 3] == LAND
				&& m_map.aMap[pTank->m_posX + 2][pTank->m_posY + 3] == LAND
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
			if (m_map.aMap[pTank->m_posX - 1][pTank->m_posY] == LAND
				&& m_map.aMap[pTank->m_posX - 1][pTank->m_posY + 1] == LAND
				&& m_map.aMap[pTank->m_posX - 1][pTank->m_posY + 2] == LAND
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
			if (m_map.aMap[pTank->m_posX + 3][pTank->m_posY] == LAND
				&& m_map.aMap[pTank->m_posX + 3][pTank->m_posY + 1] == LAND
				&& m_map.aMap[pTank->m_posX + 3][pTank->m_posY + 2] == LAND
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

void Game::BulletCollisionCheck()
{

	/*
		For each player's bullets
		index: player - i;  bullets - j; enemy - k.
	*/
	for (int i = 0; i < m_nPlayerNum; i++)
	{
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (m_pPlayer[i]->m_bullets[j].used)
			{
				int k = 0;
				switch (m_map.aMap[m_pPlayer[i]->m_bullets[j].m_posX][m_pPlayer[i]->m_bullets[j].m_posY] & 0xF0)
				{
				case PLAYER:
					//player shoot player
					m_pPlayer[i]->m_bullets[j].used = 0;

					break;
				case ENEMY:
					/*
						player shoot enemy
					*/
					PlaySoundA("sound\\pwn.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
					m_pPlayer[i]->m_bullets[j].used = 0;
					


					k = m_map.aMap[m_pPlayer[i]->m_bullets[j].m_posX][m_pPlayer[i]->m_bullets[j].m_posY] & 0x0F;
					--m_pEnemy[k]->m_nHP;
					WriteInt(MAP_LEN + 4, 7 + 5 * i, ++m_pPlayer[i]->m_nKill, COLOR_TEXT);
					UnmarkTank(m_pEnemy[k]);
					m_pEnemy[k]->ClsTank();
					break;
				case HEART:
					m_map.aMap[m_pPlayer[i]->m_bullets[j].m_posX][m_pPlayer[i]->m_bullets[j].m_posY] = LAND;
					break;
				case OBSTACLE:
					m_pPlayer[i]->m_bullets[j].used = 0;
					WriteStr(m_pPlayer[i]->m_bullets[j].m_posX, m_pPlayer[i]->m_bullets[j].m_posY, "  ", COLOR_OBSTACLE);
					break;
				case BLOCK:
					m_map.aMap[m_pPlayer[i]->m_bullets[j].m_posX][m_pPlayer[i]->m_bullets[j].m_posY] = LAND;
					m_pPlayer[i]->m_bullets[j].used = 0;
					WriteStr(m_pPlayer[i]->m_bullets[j].m_posX, m_pPlayer[i]->m_bullets[j].m_posY, "  ", COLOR_LAND);
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
			if (m_pEnemy[i]->m_bullets[j].used)
			{
				int k = 0;
				switch (m_map.aMap[m_pEnemy[i]->m_bullets[j].m_posX][m_pEnemy[i]->m_bullets[j].m_posY] & 0xF0)
				{
				case PLAYER:
					/*
						enemy shoot player
					*/
					PlaySoundA("sound\\pwn.wav", NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
					m_pEnemy[i]->m_bullets[j].used = 0;
					
					k = m_map.aMap[m_pEnemy[i]->m_bullets[j].m_posX][m_pEnemy[i]->m_bullets[j].m_posY] & 0x0F;
					--(m_pPlayer[k]->m_nHP);
					WriteInt(MAP_LEN + 4, 6 + 5 * k, m_pPlayer[k]->m_nHP, COLOR_TEXT);

					UnmarkTank(m_pPlayer[k]);
					m_pPlayer[k]->ClsTank();
					m_pPlayer[k]->m_posX = (k ? m_map.nPlayerBeginPosX[1] : m_map.nPlayerBeginPosX[0]);
					m_pPlayer[k]->m_posY = m_map.nPlayerBeginPosY;
					break;
				case ENEMY:
					//enemy shoot enemy
					m_pEnemy[i]->m_bullets[j].used = 0;
					break;
					
				case HEART:
					/*
						Game over.
					*/
					m_map.aMap[m_pEnemy[i]->m_bullets[j].m_posX][m_pEnemy[i]->m_bullets[j].m_posY] = LAND;
					break;
				case OBSTACLE:
					m_pEnemy[i]->m_bullets[j].used = 0;
					WriteStr(m_pEnemy[i]->m_bullets[j].m_posX, m_pEnemy[i]->m_bullets[j].m_posY, "  ", COLOR_OBSTACLE);
					break;
				case BLOCK:
					m_map.aMap[m_pEnemy[i]->m_bullets[j].m_posX][m_pEnemy[i]->m_bullets[j].m_posY] = LAND;
					m_pEnemy[i]->m_bullets[j].used = 0;
					WriteStr(m_pEnemy[i]->m_bullets[j].m_posX, m_pEnemy[i]->m_bullets[j].m_posY, "  ", COLOR_LAND);
					break;
				default:
					break;
				}
			}
		}
	}

	

}


void Game::MarkTank(PTank pTank,int nIndex)
{

	for (int y = pTank->m_posY; y < pTank->m_posY + 3; y++)
	{
		for (int x = pTank->m_posX; x < pTank->m_posX + 3; x++)
		{
			m_map.aMap[x][y] = (pTank->m_bIsPlayer ? PLAYER : ENEMY);
			m_map.aMap[x][y] |= nIndex;
		}
	}

}

void Game::UnmarkTank(PTank pTank)
{
	for (int y = pTank->m_oldPosY; y < pTank->m_oldPosY + 3; y++)
	{
		for (int x = pTank->m_oldPosX; x < pTank->m_oldPosX + 3; x++)
		{
			m_map.aMap[x][y] = LAND;
		}
	}
}

int Game::SaveBk()
{
	time_t t;
	struct tm lt;
	char szPath[40] = { 0 };
	FILE *fp = NULL;


	time(&t);
	localtime_s(&lt, &t);
	sprintf_s(szPath, 40, "backup\\%d_%02d_%02d_%02d_%02d_%02d.bk", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec);

	system("mkdir backup 2>nul");

	fopen_s(&fp, szPath, "wb");
	if (fp == NULL)
	{
		WriteStr(MAP_LEN / 2, WINDOW_WID / 2 - 8, "fopen_s() error\n", COLOR_TEXT);
		return -1;
	}

	fwrite(m_map.aMap, 1, sizeof(m_map.aMap), fp);
	fwrite(&m_nMission, sizeof(m_nMission), 1, fp);
	fwrite(m_pPlayer[0], sizeof(Tank), 1, fp);
	fwrite(m_pPlayer[1], sizeof(Tank), 1, fp);
	fwrite(&m_nEnemyNum, sizeof(m_nEnemyNum), 1, fp);
	for (int i = 0; i < ENEMY_NUM; ++i)
	{
		fwrite(m_pEnemy[i], sizeof(Tank), 1, fp);
	}


	fclose(fp);
	return 0;
}

int Game::LoadBk()
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
		WriteStr(30, 10, "No backup", COLOR_TEXT);
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

	fread(m_map.aMap, 1, sizeof(m_map.aMap), fp);
	fread(&m_nMission, sizeof(m_nMission), 1, fp);
	fread(m_pPlayer[0], sizeof(Tank), 1, fp);
	fread(m_pPlayer[1], sizeof(Tank), 1, fp);
	fread(&m_nEnemyNum, sizeof(m_nEnemyNum), 1, fp);
	for (int i = 0; i < ENEMY_NUM; ++i)
	{
		fread(m_pEnemy[i], sizeof(Tank), 1, fp);
	}


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

