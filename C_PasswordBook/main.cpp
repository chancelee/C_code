#include "ctrl.h"


int main()
{
	PASSWORD *pPwd = NULL;
	int nCurNum = 0;
	int op = 0;
	
	char notLogin = 1;
	while (notLogin)
	{
		puts("\t\t\t#################################################");
		puts("\t\t\t#		��ѡ��:				#\n");
		puts("\t\t\t#			1. ��  ¼		#\n");
		puts("\t\t\t#			2. ע  ��		#\n");
		puts("\t\t\t#			3. ��  ��		#\n");
		puts("\t\t\t#################################################");

		fflush(stdin);
		puts("���ѡ��");
		scanf_s("%d", &op);
		getchar();

		switch (op)
		{
		case 1:
			if (!login())
			{
				notLogin = 0;
			}			
			break;
		case 2:
			addUser();
			break;
		case 3:
			printf("�����˳�");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			exit(0);
			break;
		default:
			puts("������ѡ��");
			Sleep(1000);
			system("cls");
			break;
		}
	}

	if (readInfo(&pPwd,&nCurNum))
	{
		puts("��������\n");
	}
	
	while(1)
	{
		puts("\t\t\t#################################################");
		puts("\t\t\t#		��ѡ��:				#\n");
		puts("\t\t\t#			1. �鿴��������		#\n");
		puts("\t\t\t#			2. ��������		#\n");
		puts("\t\t\t#			3. ɾ������		#\n");
		puts("\t\t\t#			4. �޸�����		#\n");
		puts("\t\t\t#			5. ��ѯ����		#\n");
		puts("\t\t\t#			6. �����ļ�		#\n");
		puts("\t\t\t#			7. ��    ��		#\n");
		puts("\t\t\t#################################################");

		fflush(stdin);
		puts("���ѡ��");
		scanf_s("%d",&op);
		getchar();
		
		switch (op)
		{
		case SHOWALL:
			if (showAll(pPwd, nCurNum))
			{
				return -1;
			}
			break;
		case NEW:
			if (insertInfo(pPwd, &nCurNum))
			{
				return -1;
			}
			break;
		case DEL:
			if (deleteInfo(pPwd, &nCurNum))
			{
				return -1;
			}
			break;
		case ALT:
			if (alterInfo(pPwd, nCurNum))
			{
				return -1;
			}
			break;
		case SEARCH:
			if (searchInfo(pPwd, nCurNum))
			{
				return -1;
			}
			break;
		case SAVEA:
			if (saveInfoA(pPwd, nCurNum))
			{
				return - 1;
			}
			break;
		case EXIT:
			printf("�����˳�");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			exit(0);
		default:
			puts("������ѡ��");
			Sleep(1000);
			system("cls");
			break;
		}
	}

	return 0;
}
