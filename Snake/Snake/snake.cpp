
#include "snake.h"

vector<COORD> g_vSnake;
int g_nBodyLen;
BYTE g_nDir;
int g_nInternal = 200;

/*
	The snake is stored in vector<COORD>. 
	Clear the vector, then give it 3 positions as its body.
	The length and direction should also be initialized.
*/
void InitSnake()
{
	COORD pos = { 10,20 };

	while (g_vSnake.size())
	{
		vector<COORD>::iterator it = g_vSnake.begin();
		g_vSnake.erase(it);
	}

	g_vSnake.push_back(pos);
	pos.Y = 21;
	g_vSnake.push_back(pos);
	pos.Y = 22;
	g_vSnake.push_back(pos);

	g_nBodyLen = 3;
	g_nDir = UP;
}

/*
	Whether the position is in the snake.
*/
BOOL inBody(int nX, int nY,int nBodyLen)
{
	for (int i = 1; i < nBodyLen; i++)
	{
		if (g_vSnake[i].X == nX && g_vSnake[i].Y == nY)
		{
			return true;
		}
	}
	return false;
}

/*
	While moving, the "tail" should be stored as the last element in vector, but not displayed.
*/
void MoveSnake()
{
	COORD newHead = { g_vSnake[0].X, g_vSnake[0].Y };

	switch (g_nDir)
	{
	case UP:
		newHead.Y--;
		break;
	case DOWN:
		newHead.Y++;
		break;
	case LEFT:
		newHead.X--;
		break;
	case RIGHT:
		newHead.X++;
		break;
	default:
		break;
	}

	// clear the body on the screen
	for (int i = 0; i < g_nBodyLen; i++)
	{
		WriteChar(g_vSnake[i].X, g_vSnake[i].Y, "  ");
	}

	if (g_vSnake.size() == g_nBodyLen)
	{
		g_vSnake.push_back(g_vSnake[g_nBodyLen - 1]);
	}

	memcpy(&g_vSnake[1], &g_vSnake[0], (g_nBodyLen) * sizeof(COORD));
	g_vSnake[0] = newHead;

	// show the moved body
	WriteChar(g_vSnake[0].X, g_vSnake[0].Y, "  ", BACKGROUND_GREEN | BACKGROUND_RED);
	for (int i = 1; i < g_nBodyLen; i++)
	{
		WriteChar(g_vSnake[i].X, g_vSnake[i].Y, "  ",COLOR_SNAKE);
	}

}

char *snakePic =

"                                                                     \n"
"                                            ]/@@@@@@@@@@\]           \n"
"                                        ]@@@@@@@@@@@@@@@@@@@@]       \n"
"                                      /@@@@@@@@@@@@@@@@@  @@@@@\     \n"
"                                    /@@@@@@@@@@@@@@@@@     @@@@@@    \n"
"                                   /@@@@@@@@@@@@@@@@@@@@  @@@@@@@`	  \n"
"                                  =@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
"                                  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@`     \n"
"                                  @@@@@@@@@@@@@@@@@@@@@@@@@@[        \n"
"                                 =@@@@@@@@@@@/`,@@@                  \n"
"                                 ,@@@@@@@@@@@^                       \n"
"                                  @@@@@@@@@@@                        \n"
"                                  =@@@@@@@@@@                        \n"
"      =@\          ]@@@@@@@@\`     @@@@@@@@@@`                       \n"
"      =@@@^      =@@@@@@@@@@@@^    =@@@@@@@@@@                       \n"
"       @@@@\    =@@@@@@@@@@@@@@^    =@@@@@@@@@@`                     \n"
"       =@@@@\ ,/@@@@@@@@@@@@@@@O     =@@@@@@@@@@\                    \n"
"        @@@@@@@@@@@@/  @@@@@@@@@      ,@@@@@@@@@@@@`                 \n"
"        ,@@@@@@@@@@^   @@@@@@@@@        @@@@@@@@@@@@@`               \n"
"          @@@@@@@@`    @@@@@@@@@ `       @@@@@@@@@@@@@`              \n"
"            [@@[       @@@@@@@@@        ,@@@@@@@@@@@@O               \n"
"                       @@@@@@@@@         O@@@@@@@@@@@@               \n"
"                       @@@@@@@@@^      ,/@@@@@@@@@@@@@               \n"
"                       @@@@@@@@@\@@@@@@@@@@@@@@@@@@@@@@              \n"
"                         @@@@@@@@@@@@@@@@@@@@@@@@@@@ `              \n"
"                           @@@@@@@@@@@@@@@@@@@@@@@@  `               \n"
"                           \@@@@@@@@@@@@@@@@@@@@@/                 \n";