

#include "stdafx.h"
#include "Client.h"

#define MD5LEN	32

Client::Client(const char * servIP, short port)
{
	WSAData wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		MessageBoxW(NULL, L"WSAStartup() error", L"error", 0);
		return;
	}
	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, servIP, &addr.sin_addr);

	if (::connect(m_sockClient, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		MessageBoxW(NULL, L"connect() error", L"error", 0);
		closesocket(m_sockClient);
		WSACleanup();
		return;
	}

	Sleep(500);
	recv();
}

Client::~Client()
{
	for (std::vector<char*>::iterator it = m_vVirusMd5.begin();
		it < m_vVirusMd5.end();
		++it)
	{
		free(*it);
		(*it) = NULL;
	}
	m_vVirusMd5.clear();
	WSACleanup();

}



/*
*	Recv format:
*		md5 num	:	......
*/
int Client::recv()
{
	DWORD nMD5 = 0;
	char *pMD5 = NULL;

	int ret = ::recv(m_sockClient, (char*)&nMD5, sizeof(nMD5), 0);
	for (DWORD i = 0; i < nMD5; ++i)
	{
		pMD5 = (char*)calloc(1, MD5LEN + 1);
		::recv(m_sockClient, pMD5, MD5LEN, 0);
		m_vVirusMd5.push_back(pMD5);
	}

	return 0;
}

unsigned int Client::getMD5Num()
{
	return m_vVirusMd5.size();
}

char * Client::getMD5(size_t index)
{
	if (index < 0 || index >= m_vVirusMd5.size())
	{
		return nullptr;
	}
	return m_vVirusMd5[index];
}
