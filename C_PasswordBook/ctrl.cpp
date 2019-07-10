#include "ctrl.h"
#include "sha1.h"

#define XOR 0xcafebabe


int login()
{
	FILE *pPwFile = NULL;
	char szBuf[20] = { 0 };
	char sz_pw_sha1[50] = { 0 };
	char sz_pw_sha1_read[50] = { 0 };
	Sha1Digest sha1_digest;

	puts("�������û���:");
	memset(g_curUser, 0, USERNAMELEN);
	scanf_s("%s", g_curUser,USERNAMELEN);
	getchar();
	


	memset(szBuf, 0, 20);
	sprintf_s(szBuf, "%s/pw", g_curUser);
	if (fopen_s(&pPwFile, szBuf, "rb"))
	{
		puts("�û�������\n");
		Sleep(1000);
		system("cls");
		return -1;
	}

	puts("���������:");
	scanf_s("%s", szBuf, 20);
	getchar();

	sha1_digest = Sha1_get(szBuf, strlen(szBuf));
	Sha1Digest_toStr(&sha1_digest, sz_pw_sha1);

	fread(sz_pw_sha1_read, 1, sizeof(sz_pw_sha1_read), pPwFile);

	if ( strcmp(sz_pw_sha1, sz_pw_sha1_read) )
	{
		puts("�������");
		fclose(pPwFile);
		Sleep(1000);
		system("cls");
		return -1;
	}
	puts("��½�ɹ�\n");

	fclose(pPwFile);
	system("pause");
	system("cls");
	return 0;
}

int addUser()
{
	char szBuf[20] = { 0 };
	FILE *pPwFile = NULL;
	size_t pwhs = 0;
	char sz_pw_sha1[50] = { 0 };
	Sha1Digest sha1_digest;

	puts("�������û���:");
	memset(g_curUser,0, USERNAMELEN);
	scanf_s("%s",g_curUser,USERNAMELEN);
	getchar();

	sprintf_s(szBuf,"mkdir %s 2>nul",g_curUser);
	if (system(szBuf))
	{
		puts("�û��Ѵ���\n");
		Sleep(1000);
		system("cls");
		return -1;
	}


	memset(szBuf,0,20);
	sprintf_s(szBuf,"%s/pw",g_curUser);
	if(fopen_s(&pPwFile,szBuf,"wb"))
	{
		puts("addUser():fopen_s(&pPwFile,szBuf,\"wb\") error\n");
		Sleep(1000);
		system("cls");
		return -1;
	}
	
	memset(szBuf, 0, 20);
	puts("���������:");
	scanf_s("%s", szBuf, 20);
	getchar();

	sha1_digest = Sha1_get(szBuf, strlen(szBuf));
	Sha1Digest_toStr(&sha1_digest, sz_pw_sha1);

	fwrite(sz_pw_sha1,1, sizeof(sz_pw_sha1),pPwFile);
	puts("�ɹ����ɿ����ļ�\n");
	
	fclose(pPwFile);
	system("pause");
	system("cls");
	return 0;
}

int readInfo(PASSWORD** ppPwd, int *pnCurCount)
{
	FILE * pFile = NULL;
	int nSize = 0;
	char *pTemp = NULL;
	char szBuf[20] = { 0 };

	sprintf_s(szBuf,"%s/data.i",g_curUser);
	if (fopen_s(&pFile, szBuf, "rb") != 0)
	{
		nSize = sizeof(PASSWORD) * INITNUM;
		g_num = INITNUM;

		*ppPwd = (PPASSWORD)malloc(nSize);
		if (*ppPwd == NULL)
		{
			puts("malloc() error\n");
			return -1;
		}
		memset(*ppPwd, 0, nSize);

		return -1;
	}


	fread(pnCurCount, sizeof(*pnCurCount), 1, pFile);
	fread(&g_num, sizeof(g_num), 1, pFile);

	nSize = sizeof(PASSWORD) * g_num;;
	*ppPwd = (PPASSWORD)malloc(nSize);
	if (*ppPwd == NULL)
	{
		puts("malloc() error\n");
		return -1;
	}
	memset(*ppPwd, 0, nSize);

	//write
	if (fread(*ppPwd, sizeof(PASSWORD), g_num, pFile) != g_num)
	{
		puts("fread() error\n");
		fclose(pFile);
		return -1;
	}
	else
	{
		puts("data read successfully\n");
	}

	//decrypt
	pTemp = (char*)*ppPwd;
	for (size_t i = 0; i < nSize; i++)
	{
		pTemp[i] ^= XOR;
	}

	fclose(pFile);
	return 0;
}

int showAll(PASSWORD pwd[], int nCurCount)
{
	printf("================����%3d����¼================\n",nCurCount);
	for (size_t i = 0; i < nCurCount; i++)
	{
		printf("---ID:%3d -------------------------\n",i);
		printf("��վ  : %s\n", pwd[i].website);
		printf("�û���: %s\n", pwd[i].username);
		printf("����  : %s\n", pwd[i].password);
		printf("��ע  : %s\n", pwd[i].note);
	}
	puts("\n");

	system("pause");
	system("cls");
	return 0;
}

int insertInfo(PASSWORD * pwd, int * pnCurCount)
{
	int i = *pnCurCount;
	if (i < 0 || i >= g_num)
	{
		puts("���λ���쳣\n");
		return -1;
	}

	puts("��վ:");
	scanf_s("%s",pwd[i].website,100);
	getchar();
	puts("�û���:");
	scanf_s("%s", pwd[i].username, 100);
	getchar();
	puts("����:");
	scanf_s("%s", pwd[i].password, 100);
	getchar();
	puts("��ע:");
	scanf_s("%s", pwd[i].note, 100);
	getchar();

	(*pnCurCount)++;

	saveInfoB(pwd,*pnCurCount);

	system("pause");
	system("cls");

	return 0;
}

int deleteInfo(PASSWORD * pwd, int * pnCurCount)
{
	int nID2Del = -1;
	printf("����Ҫɾ����id:[0-%d]:",*pnCurCount-1);
	scanf_s("%d",&nID2Del);
	getchar();
	if (nID2Del < 0 || nID2Del >= *pnCurCount)
	{
		puts("ɾ��id�쳣\n");
		return -1;
	}

	for (size_t i = nID2Del; i < *pnCurCount -1 ; i++)
	{
		pwd[i] = pwd[i + 1];
	}
	memset(&pwd[*pnCurCount - 1],0,sizeof(PASSWORD));
	(*pnCurCount)--;

	saveInfoB(pwd,*pnCurCount);
	system("pause");
	getchar();
	system("cls");
	return 0;
}

int alterInfo(PASSWORD pwd[], int nCurCount)
{
	int nID2Alt = -1;
	printf("������Ҫ�޸ĵ�id��[0-%d]:", nCurCount - 1);
	scanf_s("%d", &nID2Alt);
	getchar();
	if (nID2Alt < 0 || nID2Alt >= nCurCount)
	{
		puts("�޸�id�쳣\n");
		return -1;
	}

	printf("---ID:%3d -------------------------\n", nID2Alt);
	printf("��վ  : %s\n", pwd[nID2Alt].website);
	printf("�û���: %s\n", pwd[nID2Alt].username);
	printf("����  : %s\n", pwd[nID2Alt].password);
	printf("��ע  : %s\n", pwd[nID2Alt].note);

	puts("---��ʼ�޸�---");
	memset(&pwd[nID2Alt],0,sizeof(PASSWORD));
	puts("��վ:");
	scanf_s("%s", pwd[nID2Alt].website,100);
	getchar();
	puts("�û���:");
	scanf_s("%s", pwd[nID2Alt].username,100);
	getchar();
	puts("����:");
	scanf_s("%s", pwd[nID2Alt].password,100);
	getchar();
	puts("��ע:");
	scanf_s("%s", pwd[nID2Alt].note,100);
	getchar();

	saveInfoB(pwd, nCurCount);
	system("pause");
	system("cls");
	return 0;
}

int searchInfo(PASSWORD pwd[], int nCurCount)
{
	char szKeyWord[100] = { 0 };
	int nFound[100] = { 0 };
	int nFoundCount = 0;

	puts("������Ҫ��ѯ�Ĺؼ��֣�\n");
	scanf_s("%s",szKeyWord,100);
	getchar();

	for (size_t i = 0; i < nCurCount; i++)
	{
		if (strstr(pwd[i].website, szKeyWord))
		{
			nFound[nFoundCount] = i;
			nFoundCount++;
			continue;
		}
		if (strstr(pwd[i].username, szKeyWord))
		{
			nFound[nFoundCount] = i;
			nFoundCount++;
			continue;
		}
		if (strstr(pwd[i].password, szKeyWord))
		{
			nFound[nFoundCount] = i;
			nFoundCount++;
			continue;
		}
		if (strstr(pwd[i].note, szKeyWord))
		{
			nFound[nFoundCount] = i;
			nFoundCount++;
			continue;
		}
	}

	printf("================��ƥ�� %3d ����¼================\n",nCurCount);
	for (size_t i = 0; i < nFoundCount; i++)
	{
		int pos = nFound[i];
		printf("---ID:%3d -------------------------\n", pos);
		printf("��վ  : %s\n", pwd[pos].website);
		printf("�û���: %s\n", pwd[pos].username);
		printf("����  : %s\n", pwd[pos].password);
		printf("��ע  : %s\n", pwd[pos].note);
	}

	system("pause");
	system("cls");

	return 0;
}

int saveInfoA(PASSWORD pwd[], int nCurCount)
{
	FILE *pFile = NULL;
	char szBuf[20] = { 0 };

	sprintf_s(szBuf, "%s/data.txt", g_curUser);
	if (fopen_s(&pFile, szBuf, "w") != 0)
	{
		puts("fopen_s() error\n");
		return -1;
	}

	fprintf(pFile,"================����%3d����¼================\n", nCurCount);
	for (size_t i = 0; i < nCurCount; i++)
	{
		fprintf(pFile,"---ID:%3d -------------------------\n", i);
		fprintf(pFile,"��վ  : %s\n", pwd[i].website);
		fprintf(pFile,"�û���: %s\n", pwd[i].username);
		fprintf(pFile,"����  : %s\n", pwd[i].password);
		fprintf(pFile,"��ע  : %s\n", pwd[i].note);
	}

	fclose(pFile);
	puts("�ѱ��棬ʹ�ú��뾡��ɾ����\n");
	system("pause");
	system("cls");
	return 0;
}

int saveInfoB(PASSWORD pwd[], int nCurCount)
{
	FILE * pFile = NULL;
	char *szBuf = NULL;
	int nSize = 0;
	char szTemp[20] = { 0 };

	sprintf_s(szTemp, "%s/data.i", g_curUser);
	if (fopen_s(&pFile, szTemp, "wb"))
	{
		puts("fopen_s() error\n");
		return -1;
	}

	fwrite(&nCurCount,sizeof(nCurCount),1,pFile);
	fwrite(&g_num,sizeof(g_num),1,pFile);

	nSize = sizeof(PASSWORD) * g_num;
	szBuf = (char*)malloc(nSize);
	memcpy_s(szBuf,nSize,pwd,nSize);

	//encrypt
	for (size_t i = 0; i < nSize; i++)
	{
		szBuf[i] ^= XOR;
	}
	
	//write
	if (fwrite(szBuf,sizeof(PASSWORD),g_num,pFile) != g_num)
	{
		puts("fwrite() error\n");
		fclose(pFile);
		return -1;
	}
	else
	{
		puts("data written successfully\n");
	}

	fclose(pFile);
	free(szBuf);
	return 0;
}
