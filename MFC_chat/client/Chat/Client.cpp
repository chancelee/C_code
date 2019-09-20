



#include "stdafx.h"
#include "Client.h"


Client g_client;

Client::Client(const char* serverIP, short port)
{
	WSAData wsaData = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		MessageBoxW(NULL, L"WSAStartup() error\n", L"ERRROR", 0);
		system("pause");
	}

	m_sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, serverIP, &addr.sin_addr);
	addr.sin_port = htons(port);


	if (::connect(m_sockClient, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		wprintf(L"connect() error: %ld ", WSAGetLastError());
		closesocket(m_sockClient);
		WSACleanup();
		system("pause");
		exit(0);
	}

}

int Client::send(DATATYPE type, const char* pData, int size)
{
	if (pData == NULL)
	{
		return -1;
	}
	CA2W wbuffer(pData);

	if (size == -1)
	{
		size = wcslen(wbuffer.m_szBuffer) * 2;
	}

	DataPackHead dataPackHead = { type, size };

	Sleep(1000);
	::send(m_sockClient, (char*)&dataPackHead, sizeof(DataPackHead), 0);
	encrypt((char*)wbuffer.m_szBuffer, size);
	::send(m_sockClient, (char*)wbuffer.m_szBuffer, size, 0);


	return 0;
}

int Client::recv(DataPack** ppDataPack)
{
	DWORD ret;
	try
	{
		DataPackHead head = {};
		DataPack *pDataPack = NULL;

		ret = ::recv(m_sockClient, (char*)&head, sizeof(head), 0);
		if (ret == 0 || ret == SOCKET_ERROR)
		{
			throw - 1;
		}

		pDataPack = (DataPack*)calloc(1, sizeof(head) + head.dataSize + 1);
		pDataPack->dataPackHead.type = head.type;

		pDataPack->dataPackHead.dataSize = head.dataSize;

		ret = ::recv(m_sockClient, pDataPack->data, pDataPack->dataPackHead.dataSize, 0);
		if (ret == 0 || ret == SOCKET_ERROR)
		{
			free(pDataPack);
			pDataPack = NULL;
			*ppDataPack = NULL;
			throw - 1;
		}
		encrypt(pDataPack->data);
		*ppDataPack = pDataPack;
	}
	catch (DWORD errcode)
	{
		return -1;
	}
	return 0;
}

void Client::cleanSocket()
{
	WSACleanup();
}

void Client::regist(const char* pNickname, const char* password)
{
	/*
	*	data format:
	*		nickname:password
	*
	*/
	CStringA buffer;
	buffer.Format("%s:%s", pNickname, password);
	send(REGISTER, buffer);
}

void Client::login(const char* pNickname, const char* password)
{
	/*
	*	data format:
	*		nickname:password
	*
	*/
	CStringA buffer;
	buffer.Format("%s:%s", pNickname, password);
	send(LOGIN, buffer);
}

//void Client::sendToFriend(const char* friendname, const char* pData)
//{
//	/*
//	*	data format:
//	*		MYNAME:friendname:data
//	*
//	*/
//	CStringA buffer;
//	buffer.Format("%s:%s:%s", m_nickname, friendname, pData);
//
//	send(SENDTOFRIEND, buffer);
//
//}

void Client::encrypt(const char *pData, int size)
{
	if (!pData) return;
	char *modify = (char*)pData;
	if (size == -1)
	{
		size = strlen(pData);
	}
	for (int i = 0; i < size; ++i)
	{
		modify[i] ^= 0x58;
	}
}


//void Client::addFriend(const char* data)
//{
//	/*
//	*	data format:
//	*		status:friendname:MYNAME
//	*	status:
//	*		 0 : add request
//	*		 1 : agree
//	*		-1 : disagree
//	*/
//
//	send(ADDFRIEND, data);
//}


//DWORD WINAPI recvProc(LPVOID arg)
//{
//	
//	DataPack * pDataPack = NULL;
//
//	while (1)
//	{
//		if (!g_client.recv(&pDataPack))
//		{
//			if (!pDataPack)
//			{
//				continue;
//			}
//			/*printf("recv from server:\n   %s\n", pDataPack->data);
//			printf("\ninput:");*/
//			switch (pDataPack->dataPackHead.type)
//			{
//			case REGISTER:
//				if (atoi(pDataPack->data))
//				{
//					MessageBoxW(NULL, L"×¢²á³É¹¦", L"", 0);
//				}
//				else
//				{
//					MessageBoxW(NULL, L"×¢²áÊ§°Ü", L"", 0);
//					//exit(0);
//				}
//				break;
//			case LOGIN:
//				if (atoi(pDataPack->data))
//				{
//					MessageBoxW(NULL, L"µÇÂ¼³É¹¦", L"", 0);
//				}
//				else
//				{
//					MessageBoxW(NULL, L"µÇÂ¼Ê§°Ü", L"", 0);
//					//exit(0);
//				}
//			}
//			free(pDataPack);
//			pDataPack = NULL;
//		}
//	}
//}
//HANDLE hRecvThread = CreateThread(
//	NULL, NULL,
//	recvProc,
//	&g_client,
//	0,
//	NULL
//);