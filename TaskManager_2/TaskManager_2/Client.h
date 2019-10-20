#pragma once
//#include <WinSock2.h>
//#pragma comment(lib, "ws2_32.lib")
#include <vector>
#include <ws2tcpip.h>

/*
*
*	  Client class is used to recv the virus md5 from the server.
*	And it's a member of CFileDlg class.
*/

#define SERVERIP "127.0.0.1"
#define PORT	8888
class Client
{
private:
	std::vector<char*> m_vVirusMd5;
public:

	Client(const char* servIP = SERVERIP, short port = PORT);
	~Client();
	SOCKET m_sockClient;

	/*
	*	Recv format:
	*		md5 num	:	......
	*/
	int recv();
	unsigned int getMD5Num();
	char * getMD5(size_t index);
};