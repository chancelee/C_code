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


Tank::Tank(BOOL bIsPlayer, int nPosX, int nPosY, int nDirection, WORD wAttr)
{
	this->m_bIsPlayer = bIsPlayer;
	this->m_posX = nPosX;
	this->m_posY = nPosY;

	this->m_oldPosX = nPosX;
	this->m_oldPosY = nPosY;

	if (bIsPlayer)
	{
		this->m_nHP = 3;
	}
	else
	{
		this->m_nHP = 0;
	}

	this->m_nDirection = nDirection;

	this->m_nKill = 0;

	this->m_wColor = wAttr;

	memset((void*)(this->m_bullets), 0, sizeof(Bullet) * BULLETSNUM_PER_TANK);
}


void Tank::DisplayTank()
{
	for (int y = this->m_posY; y < this->m_posY + 3; y++)
	{
		for (int x = this->m_posX; x < this->m_posX + 3; x++)
		{

			if (g_tankShape[this->m_nDirection][y - this->m_posY][x - this->m_posX])
			{
				WriteStr(x, y, "  ", this->m_wColor);
			}
		}
	}

	switch (this->m_nDirection)
	{
	case UP:
		WriteStr(this->m_posX + 1, this->m_posY, BARREL, COLOR_BARREL);
		break;
	case DOWN:
		WriteStr(this->m_posX + 1, this->m_posY + 2, BARREL, COLOR_BARREL);
		break;
	case LEFT:
		WriteStr(this->m_posX, this->m_posY + 1, BARREL, COLOR_BARREL);
		break;
	case RIGHT:
		WriteStr(this->m_posX + 2, this->m_posY + 1, BARREL, COLOR_BARREL);
		break;
	}
}

void Tank::ClsTank()
{
	for (int y = this->m_oldPosY; y < this->m_oldPosY + 3; y++)
	{
		for (int x = this->m_oldPosX; x < this->m_oldPosX + 3; x++)
		{
			WriteStr(x, y, "  ", COLOR_LAND);
		}
	}
}


void Tank::Fire()
{
	for (int i = 0; i < BULLETSNUM_PER_TANK; i++)
	{
		if (!this->m_bullets[i].used)
		{
			this->m_bullets[i].used = 1;
			this->m_bullets[i].m_nDirection = this->m_nDirection;

			switch (this->m_nDirection)
			{
			case UP:
				this->m_bullets[i].m_posX = this->m_posX + 1;
				this->m_bullets[i].m_posY = this->m_posY - 1;
				break;
			case DOWN:
				this->m_bullets[i].m_posX = this->m_posX + 1;
				this->m_bullets[i].m_posY = this->m_posY + 3;
				break;
			case LEFT:
				this->m_bullets[i].m_posX = this->m_posX - 1;
				this->m_bullets[i].m_posY = this->m_posY + 1;
				break;
			case RIGHT:
				this->m_bullets[i].m_posX = this->m_posX + 3;
				this->m_bullets[i].m_posY = this->m_posY + 1;
				break;
			default:
				break;
			}

			break;
		}
	}
}

void Tank::BulletMoveAndDisplay()
{

	//clean the old bullets.
	for (int i = 0; i < BULLETSNUM_PER_TANK; i++)
	{
		if (this->m_bullets[i].used)
		{
			WriteStr(this->m_bullets[i].m_posX, this->m_bullets[i].m_posY, "  ", COLOR_LAND);

			switch (this->m_bullets[i].m_nDirection)
			{
			case UP:
				--this->m_bullets[i].m_posY;
				break;
			case DOWN:
				++this->m_bullets[i].m_posY;
				break;
			case LEFT:
				this->m_bullets[i].m_posX -= 1;
				break;
			case RIGHT:
				this->m_bullets[i].m_posX += 1;
				break;
			default:
				break;
			}

			WriteStr(this->m_bullets[i].m_posX, this->m_bullets[i].m_posY, "  ", this->m_wColor);
		}
	}
}


