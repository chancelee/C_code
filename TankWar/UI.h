#pragma once
#include <windows.h>
#include <stdio.h>
#include "Attr.h"

#define WINDOW_LEN	160
#define WINDOW_WID  40

#define COLOR_TEXT_SELECTED	FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_GREEN

enum {BLANK,START, CONTIN, DRAWMP, LOADMP};

extern char *pTankPic;

/*
	Write strings and numbers on the screen.
	 wAttr parameters are provided in Attr.h and Tank.h.
*/
void WriteStr(int x, int y, char *pStr, WORD wAttr);
void WriteInt(int x, int y, int nNum, WORD wAttr);

/*
Set the window's size, mouse's attribute, and screen color.
*/
void InitWindow();

/*
	Provides 4 options for players.
	Players type UP/DOWN to select.
*/
void WelcomeAndOption(char *pChoice);