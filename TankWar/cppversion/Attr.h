#pragma once
#include <windows.h>


/*
	Map attribute index.
	For PLAYER and ENEMY, the lower 4 bits stand for the index of the tank in players or enemies array.
	For example, 0x21 means this tank is an enemy, and its index is 1 in enemies array.
*/

#define PLAYER		0x10
#define ENEMY		0x20

#define LAND		0x30
#define OBSTACLE	0x40
#define BLOCK		0x50
#define TREE		0x60
#define HEART		0x70




/*
	Map 's Color
*/
#define COLOR_LAND	BACKGROUND_INTENSITY

#define COLOR_HEART		BACKGROUND_RED

#define COLOR_OBSTACLE	BACKGROUND_GREEN | BACKGROUND_RED| BACKGROUND_BLUE | BACKGROUND_INTENSITY
#define COLOR_BLOCK		BACKGROUND_RED | BACKGROUND_GREEN
#define COLOR_TREE		BACKGROUND_GREEN

#define COLOR_TEXT		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY |COLOR_LAND	
