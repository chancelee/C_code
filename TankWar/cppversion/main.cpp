#include <stdio.h>
#include "Tank.h"
#include "Map.h"
#include "Game.h"


int main()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	Game game;
	char cChoice = 0;

	GetConsoleScreenBufferInfo(hOut, &csbi);
	InitWindow();

	while (1)
	{
		WelcomeAndOption(&cChoice);
		switch (cChoice)
		{
		case START:
			game.SetPlayerNum();
			game.m_nMission = 1;
			game.InitPlayerAndEnemy();
			game.StartGame(game.m_nMission);
			break;
		case CONTIN:
			if(game.LoadBk()) break;
			game.StartGame(-1);
			break;
		case DRAWMP:
			game.m_map.DrawMap();
			break;
		case LOADMP:
			game.SetPlayerNum();
			game.InitPlayerAndEnemy();
			game.StartGame(0);
			break;
		default:
			break;
		}
		system("cls");
	}
	
	SetConsoleTextAttribute(hOut,csbi.wAttributes);
	return 0;
}