/*
	Set two time counters at (1,1) and (1,10).
	(1,1) increase 1 per second, and (1,10) per half second.
*/

#include <windows.h>
#include <stdio.h>
#include <time.h>

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);


void WriteChar(int x, int y, char *pStr, WORD wAttr)
{
	CONSOLE_CURSOR_INFO cci = {1,false};
	COORD pos = {x * 2, y};

	SetConsoleCursorInfo(hOut,&cci);
	SetConsoleTextAttribute(hOut,wAttr);
	SetConsoleCursorPosition(hOut,pos);

	printf(pStr);
	
}

int main()
{
	char szNum1[10] = {0}, szNum2[10] = { 0 };
	int nNum1 = 0, nNum2 = 0;

	clock_t time1_start = clock();
	clock_t time1_end;
	clock_t time2_start = clock();
	clock_t time2_end;

	while (1)
	{
		time1_end = clock();
		if (time1_end - time1_start > 1000)
		{
			time1_start = time1_end;
			sprintf_s(szNum1,10,"%d",nNum1++);
			WriteChar(1,1,szNum1,FOREGROUND_RED);
		}

		time2_end = clock();
		if (time2_end - time2_start > 500)
		{
			time2_start = time2_end;
			sprintf_s(szNum2, 10, "%d", nNum2++);
			WriteChar(1, 10, szNum2, FOREGROUND_RED);
		}
	}

	system("pause");
	return 0;
}

