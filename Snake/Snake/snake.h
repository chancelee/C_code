#pragma once

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "WriteChar.h"
#include "Att.h"

using namespace std;

#define UP		0
#define DOWN	1
#define LEFT	2
#define RIGHT	3

extern vector<COORD> g_vSnake;
extern int g_nBodyLen;
extern BYTE g_nDir;
extern int g_nInternal;
extern char *snakePic;

void InitSnake();
BOOL inBody(int nX, int nY, int nBodyLen);
void MoveSnake();
