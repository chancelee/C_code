#pragma once

#include <time.h>
#include <conio.h>

#include "Attr.h"
#include "Tank.h"
#include "Map.h"




#define ENEMY_NUM	6


void SetPlayerNum(int *pPlayerNum);
void InitPlayersAndEnemies(PTank pPlayer,int nPlayerNum,PTank pEnemy);

/*
	Single player mode.
*/
void StartGame(int nMission, PTank pPlayer, int nPlayerNum, PTank pEnemy);

/*
*/
int MoveInMap(PTank pTank,BYTE dir);

/*
	Check the collision of bullet and process it.
*/
void BulletCollisionCheck(PTank pTank, int nPlayerNum, PTank pEnemy);

/*
	Mark the position of tank on the 2-dimension map.
	PLAYER | nIndex or ENEMY | nIndex will assigned to the tank's position on the map, which is useful for bullet collision checking.
*/
void MarkTank(PTank pTank, int nIndex);
/*
	Set the old position of tank on the 2-dimension map to LAND.
*/
void UnmarkTank(PTank pTank);

/*
	Save and load a backup.
	The data includes mission, map, players and enemies.
	The .bk file is named by local time , and be stored in "backup" directory.
*/
int SaveBk(int nMission, PTank pPlayer, PTank pEnemy);
int LoadBk(int *pnMission, PTank pPlayer, PTank pEnemy);

