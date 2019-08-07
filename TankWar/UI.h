#pragma once
#include <windows.h>
#include <stdio.h>
#include "Attr.h"

#define WINDOW_LEN	160
#define WINDOW_WID  40

#define START	1
#define CONTIN	2
#define DRAWMP	3
#define LOADMP	4


#define COLOR_TEXT_SELECTED	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_GREEN

extern char *pTankPic;

void WriteChar(int x, int y, char *pStr, WORD wAttr);
void WriteInt(int x, int y, int nNum, WORD wAttr);

void InitWindow();
void WelcomeAndOption(char *pChoice);