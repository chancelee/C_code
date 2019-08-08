#pragma once

#include <time.h>
#include <io.h>
#include <stdlib.h>
#include <direct.h>

#include "UI.h"
#include "Attr.h"

#define MAP_LEN		48
#define MAP_WID		28

/*
	The max num of stored maps. 
*/
#define MAP_NUM		5




class Map
{
public:
	BYTE aMap[MAP_LEN][MAP_WID];

	/*
		Where players and enemies begin.
	*/
	int nPlayerBeginPosY;
	int nPlayerBeginPosX[2];
	int nEnemyBeginPosY;
	int nEnemyBeginPosX[3];
public:
	Map();

	/*
		Macro COLORs in Attr.h will used.
	*/
	void DisplayMap();

	/*
	The map file is named by the local time, and be stored in "map" directory.
	*/
	int SaveMap();

	/*
	If nMission > 0, function will load the corresponding mission's map in "mission" directory.
	Else if nMission == 0, a player-designed map in "map" directory will be selected and loaded.
	Else , the map has been loaded.
	*/
	int LoadMap(int nMission);

	/*
	Player call this func to design a map.
	Type UP/DOWN to select BLOCK/OBSTACLE.
	Click right button to set LAND.
	Type ENTER will call SaveMap() to store it in "map" directory.
	*/
	int DrawMap();
};







