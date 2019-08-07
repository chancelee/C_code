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
	Mark and unmark the position of tank on the 2-dimension map
*/
void MarkTank(PTank pTank);
void UnmarkTank(PTank pTank);

int SaveBk(int nMission, PTank pPlayer, PTank pEnemy);
int LoadBk(int *pnMission, PTank pPlayer, PTank pEnemy);

