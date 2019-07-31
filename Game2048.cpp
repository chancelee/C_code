/*
	Game 2048
	2019/7/31
*/

#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>

int g_map[4][4] = { 0 };
int g_score = 0;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

void DrawRect(char *szBuf, SMALL_RECT rect, DWORD forecolor, DWORD backcolor)
{
	COORD pos = { 0 };
	DWORD dSize = 0;

	pos.X = rect.Left + (rect.Right - rect.Left) / 2 - strlen(szBuf) / 2;
	pos.Y = rect.Top + (rect.Bottom - rect.Top) / 2;

	SetConsoleCursorPosition(hOut, pos);
	printf(szBuf);

	for (int i = 0; i < rect.Bottom - rect.Top; i++)
	{
		pos.X = rect.Left;
		pos.Y = rect.Top + i;
		SetConsoleCursorPosition(hOut, pos);
		FillConsoleOutputAttribute(hOut,forecolor | backcolor, rect.Right - rect.Left, pos,&dSize);
	}
	
}

void CreateNumber()
{
	int x = 0;
	int y = 0;
	int nNum = 0;

	srand((unsigned int)time(0));
	x = rand() % 4;
	y = rand() % 4;
	nNum = rand() % 2 ? 2 : 4;

	do
	{
		if (g_map[x][y] == 0)
		{
			g_map[x][y] = nNum;
			break;
		}
		x = rand() % 4;
		y = rand() % 4;
	} while (1);
}

void DrawNumber(int nNum, SMALL_RECT rect)
{
	switch(nNum)
	{
	case 0:
		DrawRect("    ", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 2:
		DrawRect(" 02 ", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 4:
		DrawRect(" 04 ", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 8:
		DrawRect(" 08 ", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 16:
		DrawRect(" 16 ", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 32:
		DrawRect(" 32 ", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 64:
		DrawRect(" 64 ", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 128:
		DrawRect("0128", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 256:
		DrawRect("0256", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 512:
		DrawRect("0512", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 1024:
		DrawRect("1024", rect, FOREGROUND_BLUE, BACKGROUND_RED);
		break;
	case 2048:
		break;
		DrawRect("2048", rect, FOREGROUND_BLUE, BACKGROUND_RED);
	default:
		break;
	}
}

void DrawMap()
{
	SMALL_RECT rect = { 0};
	for (int i = 0; i < 4; i++)
	{
		rect.Left = (i + 2) * 6;
		rect.Right = rect.Left + 6;
		for (int j = 0; j < 4; j++)
		{
			rect.Top = (j + 1) * 3;
			rect.Bottom = rect.Top + 3;

			DrawNumber(g_map[j][i],rect);
		}
	}
}

void DrawScore()
{
	COORD pos = { 12,2 };
	DWORD dwSize = 0;

	SetConsoleCursorPosition(hOut,pos);
	printf("%d",g_score);
	FillConsoleOutputAttribute(hOut,FOREGROUND_GREEN, 10,pos,&dwSize);

}

bool Up()
{
	bool bIsMove = false;

	for (int i = 0; i < 4; i++)
	{

		//colomn
		for (int j = 0; j < 4; j++)
		{
			//search below
			// there is a num
			if(g_map[j][i] != 0)
			{
				for (int k = j + 1; k < 4; k++)
				{
					//the same num 
					if(g_map[k][i] == g_map[j][i])
					{
						g_map[j][i] *= 2;
						g_map[k][i] = 0;
						g_score += g_map[j][i];
						bIsMove = true;
						break;
					}
					else if(g_map[k][i] != 0)
					{
						break;
					}
				}
				
			}
			// no num, search again
			else
			{
				for (int k = j + 1; k < 4; k++)
				{
					if (g_map[k][i] != 0)
					{
						g_map[j][i] = g_map[k][i];
						g_map[k][i] = 0;
						j++;
						bIsMove = true;
						break;
					}
				}
			}
		}
	}

	return bIsMove;
}

bool Down()
{
	bool bIsMove = false;


	for (int i = 0; i < 4; i++)
	{

		//colomn
		for (int j = 3; j >= 0; j--)
		{
			//search upper
			// there is a num
			if (g_map[j][i] != 0)
			{
				for (int k = j - 1; k >= 0; k--)
				{
					//the same num 
					if (g_map[k][i] == g_map[j][i])
					{
						g_map[j][i] *= 2;
						g_map[k][i] = 0;
						g_score += g_map[j][i];
						bIsMove = true;
						break;
					}
					else if (g_map[k][i] != 0)
					{
						break;
					}
				}

			}
			// no num, search again
			else
			{
				for (int k = j - 1; k >= 0; k--)
				{
					if (g_map[k][i] != 0)
					{
						g_map[j][i] = g_map[k][i];
						g_map[k][i] = 0;
						j--;
						bIsMove = true;
						break;
					}
				}
			}
		}
	}

	return bIsMove;
}

bool Left()
{
	bool bIsMove = false;

	for (int i = 0; i < 4; i++)
	{
		//colomn
		for (int j = 0; j < 4; j++)
		{
			//search right
			// there is a num
			if (g_map[i][j] != 0)
			{
				for (int k = j + 1; k < 4; k++)
				{
					//the same num 
					if (g_map[i][k] == g_map[i][j])
					{
						g_map[i][j] *= 2;
						g_map[i][k] = 0;
						g_score += g_map[i][j];
						bIsMove = true;
						break;
					}
					else if (g_map[i][k] != 0)
					{
						break;
					}
				}

			}
			// no num, search again
			else
			{
				for (int k = j + 1; k < 4; k++)
				{
					if (g_map[i][k] != 0)
					{
						g_map[i][j] = g_map[i][k];
						g_map[i][k] = 0;
						j++;
						bIsMove = true;
						break;
					}
				}
			}
		}
	}

	return bIsMove;
}

bool Right()
{
	bool bIsMove = false;

	for (int i = 0; i < 4; i++)
	{
		//colomn
		for (int j = 3; j >= 0; j--)
		{
			//search left
			// there is a num
			if (g_map[i][j] != 0)
			{
				for (int k = j - 1; k >= 0; k--)
				{
					//the same num 
					if (g_map[i][k] == g_map[i][j])
					{
						g_map[i][j] *= 2;
						g_map[i][k] = 0;
						g_score += g_map[i][j];
						bIsMove = true;
						break;
					}
					else if (g_map[i][k] != 0)
					{
						break;
					}
				}

			}
			// no num, search again
			else
			{
				for (int k = j - 1; k >= 0; k--)
				{
					if (g_map[i][k] != 0)
					{
						g_map[i][j] = g_map[i][k];
						g_map[i][k] = 0;
						j--;
						bIsMove = true;
						break;
					}
				}
			}
		}
	}

	return bIsMove;
}

void StartGame()
{
	char cOp = 0;
	bool bIsMove = false;
	bool bExit = false;

	// 1. generate a random num
	CreateNumber();
	// 2. display the score
	DrawScore();
	// 3. draw map
	DrawMap();

	// 4. get the keyboard input
	while (1)
	{
		cOp = _getch();
		switch (cOp)
		{
		case 'w':
			bIsMove = Up();
			break;
		case 's':
			bIsMove = Down();
			break;
		case 'a':
			bIsMove = Left();
			break;
		case 'd':
			bIsMove = Right();
			break;
		default:
			bIsMove = false;
			break;
		}

		if (bExit)
			break;
		if (bIsMove)
		{
			CreateNumber();
			DrawMap();
			DrawScore();
		}
	}


	// 5. exit

}

int main()
{
	StartGame();
	return 0;
}
