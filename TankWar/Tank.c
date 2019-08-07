#include "Tank.h"


char g_tankShape[4][3][3] =
{
	//up
	{
		{ 0,1,0 },
		{ 1,1,1 },
		{ 1,0,1 },
	},
	//down
	{
		{ 1,0,1 },
		{ 1,1,1 },
		{ 0,1,0 },
	},
	//left
	{
		{ 0,1,1 },
		{ 1,1,0 },
		{ 0,1,1 },
	},
	//right
	{
		{ 1,1,0 },
		{ 0,1,1 },
		{ 1,1,0 },
	}
};


void InitTank(PTank pTank, BOOL bIsPlayer, int nPosX, int nPosY, int nDirection, WORD wAttr)
{
	pTank->m_bIsPlayer = bIsPlayer;
	pTank->m_posX = nPosX;
	pTank->m_posY = nPosY;

	pTank->m_oldPosX = nPosX;
	pTank->m_oldPosY = nPosY;

	if (bIsPlayer)
	{
		pTank->m_nHP = 3;
	}
	else
	{
		pTank->m_nHP = 0;
	}

	pTank->m_nDirection = nDirection;

	pTank->m_nKill = 0;

	pTank->m_wColor = wAttr;

	memset((void*)(pTank->m_bullets), 0, sizeof(Bullet) * BULLETSNUM_PER_TANK);
}

void DisplayTank(PTank pTank)
{
	for (int y = pTank->m_posY; y < pTank->m_posY + 3; y++)
	{
		for (int x = pTank->m_posX; x < pTank->m_posX + 3; x++)
		{

			if (g_tankShape[pTank->m_nDirection][y - pTank->m_posY][x - pTank->m_posX])
			{
				WriteChar(x, y, "  ", pTank->m_wColor);
			}
		}
	}

	switch (pTank->m_nDirection)
	{
	case UP:
		WriteChar(pTank->m_posX + 1, pTank->m_posY, BARREL, COLOR_BARREL);
		break;
	case DOWN:
		WriteChar(pTank->m_posX + 1, pTank->m_posY + 2, BARREL, COLOR_BARREL);
		break;
	case LEFT:
		WriteChar(pTank->m_posX, pTank->m_posY + 1, BARREL, COLOR_BARREL);
		break;
	case RIGHT:
		WriteChar(pTank->m_posX + 2, pTank->m_posY + 1, BARREL, COLOR_BARREL);
		break;
	}
}

void ClsTank(PTank pTank)
{
	for (int y = pTank->m_oldPosY; y < pTank->m_oldPosY + 3; y++)
	{
		for (int x = pTank->m_oldPosX; x < pTank->m_oldPosX + 3; x++)
		{
			WriteChar(x, y, "  ", COLOR_LAND);
		}
	}

}

void Fire(PTank pTank)
{
	for (int i = 0; i < BULLETSNUM_PER_TANK; i++)
	{
		if (!pTank->m_bullets[i].used)
		{
			pTank->m_bullets[i].used = 1;
			pTank->m_bullets[i].m_nDirection = pTank->m_nDirection;

			switch (pTank->m_nDirection)
			{
			case UP:
				pTank->m_bullets[i].m_posX = pTank->m_posX + 1;
				pTank->m_bullets[i].m_posY = pTank->m_posY - 1;
				break;
			case DOWN:
				pTank->m_bullets[i].m_posX = pTank->m_posX + 1;
				pTank->m_bullets[i].m_posY = pTank->m_posY + 3;
				break;
			case LEFT:
				pTank->m_bullets[i].m_posX = pTank->m_posX - 1;
				pTank->m_bullets[i].m_posY = pTank->m_posY + 1;
				break;
			case RIGHT:
				pTank->m_bullets[i].m_posX = pTank->m_posX + 3;
				pTank->m_bullets[i].m_posY = pTank->m_posY + 1;
				break;
			default:
				break;
			}

			break;
		}
	}
}

void BulletMoveAndDisplay(PTank pPlayer, int nPlayerNum, PTank pEnemy, int nEnemyNum)
{
	/*
		Clean and move the bullets of players.
	*/
	for (int i = 0; i < nPlayerNum; i++)
	{
		//clean the old bullets.
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pPlayer[i].m_bullets[j].used)
			{
				WriteChar(pPlayer[i].m_bullets[j].m_posX, pPlayer[i].m_bullets[j].m_posY, "  ", COLOR_LAND);
			}
		}

		//move the bullets.
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pPlayer[i].m_bullets[j].used)
			{
				switch (pPlayer[i].m_bullets[j].m_nDirection)
				{
				case UP:
					--pPlayer[i].m_bullets[j].m_posY;
					break;
				case DOWN:
					++pPlayer[i].m_bullets[j].m_posY;
					break;
				case LEFT:
					pPlayer[i].m_bullets[j].m_posX -= 1;
					break;
				case RIGHT:
					pPlayer[i].m_bullets[j].m_posX += 1;
					break;
				default:
					break;
				}
			}
		}

		//show the new positon of bullets.
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pPlayer[i].m_bullets[j].used)
			{
				WriteChar(pPlayer[i].m_bullets[j].m_posX, pPlayer[i].m_bullets[j].m_posY, "  ", pPlayer[i].m_wColor);
			}
		}
	}

	/*
	Clean and move the bullets of enemies.
	*/
	for (int i = 0; i < nEnemyNum; i++)
	{
		//clean the old bullets.
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pEnemy[i].m_bullets[j].used)
			{
				WriteChar(pEnemy[i].m_bullets[j].m_posX, pEnemy[i].m_bullets[j].m_posY, "  ", COLOR_LAND);
			}
		}

		//move the bullets.
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pEnemy[i].m_bullets[j].used)
			{
				switch (pEnemy[i].m_bullets[j].m_nDirection)
				{
				case UP:
					--pEnemy[i].m_bullets[j].m_posY;
					break;
				case DOWN:
					++pEnemy[i].m_bullets[j].m_posY;
					break;
				case LEFT:
					pEnemy[i].m_bullets[j].m_posX -= 1;
					break;
				case RIGHT:
					pEnemy[i].m_bullets[j].m_posX += 1;
					break;
				default:
					break;
				}
			}
		}

		//show the new positon of bullets.
		for (int j = 0; j < BULLETSNUM_PER_TANK; j++)
		{
			if (pEnemy[i].m_bullets[j].used)
			{
				WriteChar(pEnemy[i].m_bullets[j].m_posX, pEnemy[i].m_bullets[j].m_posY, "  ", pEnemy[i].m_wColor);
			}
		}
	}
}


