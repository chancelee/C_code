#pragma once

#define SHOWALL		1
#define NEW			2
#define DEL			3
#define ALT			4
#define SEARCH		5
#define SAVEA		6
#define EXIT		7
#define SAVEB		8

typedef struct {
	char website[100];
	char username[100];
	char password[100];
	char note[100];
}PASSWORD,*PPASSWORD;

#define INITNUM		100
#define USERNAMELEN 30
extern int g_num;	//current num 
extern char g_curUser[USERNAMELEN];