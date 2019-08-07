#pragma once

#include <time.h>
#include <io.h>
#include <stdlib.h>
#include <direct.h>

#include "UI.h"
#include "Attr.h"

#define MAP_LEN		48
#define MAP_WID		28

#define MAP_NUM		5
/*
	Where players and enemies begin.
*/
extern int g_playerBeginPosY;
extern int g_playerBeginPosX[2];
extern int g_enemyBeginPosY;
extern int g_enemyBeginPosX[3];


extern BYTE g_map[MAP_LEN][MAP_WID];

void InitMap();
void DisplayMap();

int SaveMap();
int LoadMap(int nMission);
int DrawMap();