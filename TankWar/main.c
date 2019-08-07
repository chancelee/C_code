#include <stdio.h>
#include "Tank.h"
#include "Map.h"
#include "Ctrl.h"


int main()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	char cChoice = 0;

	Tank players[2] = { 0 };
	Tank enemies[ENEMY_NUM] = { 0 };
	int nPlayerNum = 1, nMission = 1;;

	GetConsoleScreenBufferInfo(hOut, &csbi);
	InitWindow();

	while (1)
	{
		WelcomeAndOption(&cChoice);
		switch (cChoice)
		{
		case START:
			SetPlayerNum(&nPlayerNum);
			InitPlayersAndEnemies(players,nPlayerNum,enemies);
			StartGame(1,players,nPlayerNum,enemies);
			break;
		case CONTIN:
			if(LoadBk(&nMission,players,enemies)) break;
			StartGame(1, players, nPlayerNum, enemies);
			break;
		case DRAWMP:
			DrawMap();
			break;
		case LOADMP:
			//LoadMap(0);
			SetPlayerNum(&nPlayerNum);
			InitPlayersAndEnemies(players, nPlayerNum, enemies);
			StartGame(-1,players,nPlayerNum,enemies);
			break;
		default:
			break;
		}
		system("cls");
	}

	SetConsoleTextAttribute(hOut,csbi.wAttributes);
	return 0;
}