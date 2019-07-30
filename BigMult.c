#include<stdio.h>
#include<string.h>
#include<stdlib.h>


char * bigMult(char *pNum1, int nLen1, char *pNum2, int nLen2)
{
	int nLen = nLen1 + nLen2;
	char *pRes = (char*)malloc(nLen + 1);
	memset(pRes,0,sizeof(int) * nLen);

	for (int i = nLen2 - 1; i >= 0 ; i--)
	{
		int nResInd = nLen - (nLen2 - i);
		for (int j = nLen1 - 1; j >= 0; j--)
		{
			pRes[nResInd] += (pNum2[i] - '0') * (pNum1[j] - '0');
			nResInd--;
		}
	}
	for(int i = nLen - 1; i > 0; i--)
	{
		if (pRes[i]  / 10)
		{
			pRes[i - 1] += pRes[i] / 10;
			pRes[i] -= pRes[i] / 10 * 10;
		}
	}
	for (int i = 0; i < nLen; i++)
	{
		pRes[i] += '0';
	}

	return pRes;
}

int main()
{
	char *pNum1 = "3456", *pNum2 = "59";
	char *pRes = bigMult(pNum1, strlen(pNum1), pNum2, strlen(pNum2));
	char *pTemp = pRes;

	while (*pTemp == '0') pTemp++;
	printf("%s\n",pTemp);
	
	free(pRes);


	return 0;
}
