#pragma once

#include <stdio.h>
#include <time.h>
#include "UI.h"
#include "Attr.h"

/*
	Directions
*/
#define UP			0
#define DOWN		1
#define LEFT		2
#define RIGHT		3

/*
	Tank 's attributes
*/
#define BULLETSNUM_PER_TANK 3
#define TANK_SPEED	120
#define BULLET_SPEED (TANK_SPEED / 2)
#define BARREL	"++"

/*
	Tank 's color
*/
#define COLOR_PLAYER1	BACKGROUND_RED
#define COLOR_PLAYER2	BACKGROUND_BLUE
#define COLOR_ENEMY		BACKGROUND_BLUE | BACKGROUND_RED
#define COLOR_BARREL	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | COLOR_PLAYER1

extern char g_tankShape[4][3][3];

/*
	The struct of tank, including position,level,direction and score etc...
*/
typedef struct _Bullet
{
	BYTE		 used;
	unsigned int m_posX;
	unsigned int m_posY;

	unsigned int m_nDirection;

}Bullet, *PBullet;

typedef struct _TANK
{
	BOOL		 m_bIsPlayer;
	unsigned int m_posX;
	unsigned int m_posY;
	unsigned int m_oldPosX;
	unsigned int m_oldPosY;

	unsigned int m_nLevel;
	
	unsigned int m_nHP;
	
	unsigned int m_nDirection;
	
	unsigned int m_nKill;

	WORD		 m_wColor;

	Bullet m_bullets[BULLETSNUM_PER_TANK];

}Tank,*PTank;




/*
	Constructor of tank.
	bIsPlayer : PLAYER or ENEMY.
*/
void InitTank(PTank pTank,BOOL bIsPlayer, int nPosX, int nPosY, int nDirection, WORD wAttr);

void DisplayTank(PTank pTank);
void ClsTank(PTank pTank);

void Fire(PTank pTank);
void BulletMoveAndDisplay(PTank pTank, int nPlayerNum, PTank pEnemy, int nEnemyNum);

BOOL inBody(PTank pTank, unsigned int x, unsigned int y);
