#pragma once

#include <io.h>
#include <windows.h>
#include "snake.h"
#include "Map.h"
#include "data.h"

extern int g_nScore;

void InitWindow();
void WelcomeAndOption(char &cChoice);
void InfoBox();
void ChooseSpeed(int &nSpeed);

void SingleBegin();
void GetPlayerInput();

int ScoreOp();
void CreateFood();
int SaveBk();
int LoadBk();