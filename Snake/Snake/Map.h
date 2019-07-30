#pragma once

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <io.h>
#include <vector>
#include <string>

#include "WriteChar.h"
#include "Att.h"

using namespace std;

#define MAP_LEN		48
#define MAP_WID		28


extern BYTE g_map[MAP_LEN][MAP_WID];

void InitMap();
void GenMap();
int DrawMap();
int LoadMap();
