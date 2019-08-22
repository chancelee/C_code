#pragma once

#include <stdio.h>
#include <time.h>
#include "UI.h"
#include "Attr.h"


/*
Tank 's attributes
*/


/*
	Direction attributes.
*/
#define UP			0
#define DOWN		1
#define LEFT		2
#define RIGHT		3


/*
	Number attributes.
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

/*
	Tank's shape is controlled by 4 2-dimension arrays. 
*/
extern char g_tankShape[4][3][3];


/*
	The struct of bullets, which will be initialized in Tank's constructor.
	When the tank fires, "used" will be set.
*/
class Bullet
{
public:
	BYTE		 used;
	unsigned int m_posX;
	unsigned int m_posY;

	unsigned int m_nDirection;

};


/*
	The struct of tank, including position,direction and score etc...
	Tank's old positions are also stored to be cleand while moving.
*/
class Tank
{
public:
	BOOL		 m_bIsPlayer;
	unsigned int m_posX;
	unsigned int m_posY;
	unsigned int m_oldPosX;
	unsigned int m_oldPosY;

	unsigned int m_nHP;
	
	unsigned int m_nDirection;
	
	unsigned int m_nKill;

	WORD		 m_wColor;

	Bullet m_bullets[BULLETSNUM_PER_TANK];
public:
	/*
		Constructor of tank.
		bIsPlayer : PLAYER or ENEMY.
	*/
	Tank(BOOL bIsPlayer, int nPosX, int nPosY, int nDirection, WORD wAttr);
	void InitTank(int nPosX, int nPosY, int nDirection);

	/*
		Display and Clean one tank on the screen.
	*/
	void DisplayTank();
	void ClsTank();

	/*
		Activate the tank's unused bullet, and set the bullet's direction and positions.
	*/
	void Fire();

	/*
		This function should be called after bullet collision check.
		Every player and enemy's bullets will be checked in this function, if any bullet is used, its old position will cleaned , then its new position will be displayed.
	*/
	void BulletMoveAndDisplay();
};

typedef Tank *PTank;






