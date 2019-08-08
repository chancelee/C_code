#pragma once

#include <time.h>
#include <conio.h>

#include "Attr.h"
#include "Tank.h"
#include "Map.h"


#define ENEMY_NUM	6


class Game
{
public:
	Map m_map;
	PTank m_pPlayer[2];
	PTank m_pEnemy[ENEMY_NUM];
	int m_nPlayerNum, m_nMission;
public:
	Game();


	/*
	Init the players and enemies after the number of players is set.
	*/
	void SetPlayerNum();
	void InitPlayersAndEnemies();

	/*
	Game procedure:
	1. Check whether all missions are completed.
	2. Load and display the mission or player-designed map.
	3. Display player's tank.
	4. Enter the main loop.
	Conditions: players are alive, heart not destroyed and any enemy alive.
	5. If players win, enter next mission or return (nMission < 1).
	Else, lose and return.

	*/
	void StartGame(int nMission);

	/*
	If the tank move forward or change its direction successfully, return TRUE.
	Else if collision happens, return FALSE;
	*/
	BOOL MoveInMap(PTank pTank, BYTE dir);

	/*
	Check the collision of bullet and process it.
	*/
	void BulletCollisionCheck();

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
	int SaveBk();
	int LoadBk();


};
