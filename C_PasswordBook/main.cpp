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
		puts("\t\t\t#		请选择:				#\n");
		puts("\t\t\t#			1. 登  录		#\n");
		puts("\t\t\t#			2. 注  册		#\n");
		puts("\t\t\t#			3. 退  出		#\n");
		puts("\t\t\t#################################################");

		fflush(stdin);
		puts("你的选择：");
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
			printf("正在退出");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			exit(0);
			break;
		default:
			puts("请重新选择");
			Sleep(1000);
			system("cls");
			break;
		}
	}

	if (readInfo(&pPwd,&nCurNum))
	{
		puts("暂无数据\n");
	}
	
	while(1)
	{
		puts("\t\t\t#################################################");
		puts("\t\t\t#		请选择:				#\n");
		puts("\t\t\t#			1. 查看所有密码		#\n");
		puts("\t\t\t#			2. 新增密码		#\n");
		puts("\t\t\t#			3. 删除密码		#\n");
		puts("\t\t\t#			4. 修改密码		#\n");
		puts("\t\t\t#			5. 查询密码		#\n");
		puts("\t\t\t#			6. 生成文件		#\n");
		puts("\t\t\t#			7. 退    出		#\n");
		puts("\t\t\t#################################################");

		fflush(stdin);
		puts("你的选择：");
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
			printf("正在退出");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			Sleep(1000);
			printf(".");
			exit(0);
		default:
			puts("请重新选择");
			Sleep(1000);
			system("cls");
			break;
		}
	}

	return 0;
}
