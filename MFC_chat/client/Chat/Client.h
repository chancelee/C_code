#pragma once
#pragma once

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <AtlBase.h>
#include <iostream>
#include <stdlib.h>
#include <atlstr.h>
#include <ws2tcpip.h>
#include <windows.h>

enum ERRORTYPE { STARTERROR, SOCKETERROR, CONNECTERROR };
enum DATATYPE { REGISTER, LOGIN, 
	SENDTOFRIEND, GROUPMSG, 
	ADDFRIEND, SHOWFRIEND,
	CREATEGROUP, JOINGROUP, SHOWGROUP};

enum ADDFRIEND{REQUEST, AGREE, DISAGREE};

#define SERVERIP "127.0.0.1"
#define PORT	8888

#pragma pack(push, 1)
typedef struct {
	DATATYPE type;
	int dataSize;
	//	char data[1];
}DataPackHead;
typedef struct {
	DataPackHead dataPackHead;
	char data[1];
}DataPack;
#pragma pack(pop)


class Client
{
public:
	char m_nickname[20];
	SOCKET m_sockClient;
public:
	Client(const char* serverIP = SERVERIP, short port = PORT);
	void cleanSocket();

	//int connect();

	int send(DATATYPE type, const char* pData, int size = -1);

	int recv(DataPack** ppDataPack);

	void regist(const char* pNickname, const char* password);
	void login(const char* pNickname, const char* password);
	//void sendToFriend(const char* friendname, const char* pData);
	//void addFriend(const char* friendname);
private:
	void encrypt(const char *pData, int size = -1);

};


//DWORD WINAPI recvProc(LPVOID arg);
extern Client g_client;