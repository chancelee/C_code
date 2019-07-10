#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include "data.h"


int login();
int addUser();

int readInfo(PASSWORD** ppPwd,int *pnCount);

int showAll(PASSWORD pwd[], int nCount);
int insertInfo(PASSWORD pwd[], int *pnCount);
int deleteInfo(PASSWORD pwd[], int *pnCount);
int alterInfo(PASSWORD pwd[], int nCount);
int searchInfo(PASSWORD pwd[], int nCount);
int saveInfoA(PASSWORD pwd[], int nCount);
int saveInfoB(PASSWORD pwd[], int nCount);
