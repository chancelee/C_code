#pragma once

#include <windows.h>
#include <stdio.h>

void WriteChar(int nX, int nY, const char* str, WORD wAttributes = 0);

void WriteInt(int nX, int nY, int nNum, WORD wAttributes = 0);
