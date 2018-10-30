#include<stdio.h>


#include "miracl.h"
#include<math.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	

	int m,k,kx;
	int a;
	int g;
	int r;
	double p;
	//随机数种子为时间
	miracl *mip = mirsys(40000,10);
	//功能说明: 初始化MIRACL系统，该函数必须在调用MIRACL库函数之前先执行	
	//初始化400位的10进制数
	
	big bx = mirvar(0);
	//初始化bx=0

	//printf("2.c\n");
	/*
	printf("intput m(m>=3):");
	scanf("%d",&m);
	getchar();
	printf("m=:%d\n",m);

	printf("intput k:");
	scanf("%d",&k);
	getchar();
	printf("k=:%d\n",k);
	*/
	FILE* fp = fopen("data.txt","r");
	fscanf(fp,"%d",&m);
	fscanf(fp,"%d",&k);

	printf("m=:%d\n",m);
	printf("k=:%d\n",k);
	
	
	kx = 1;
	srand(time(0));
	//用时间戳设置随机数种子
	for(a=2;(a <= m-2 ) && ( kx <= k);)
	{
		a = (rand()+2)%(m-1);
		
		g = igcd(a,m);
		if(g == 1)
		{
			expint(a,m-1,bx);
			//bx = a**(m-1)
			r = subdiv(bx,m,bx);
			//r = bx % m
			
			if(r == 1)
			{
				
				p = 1-(1/pow(2,kx));
				
				printf("a=%d, (%d,%d)=1,%d(%d-1)(mod %d)≡1，故m=%d可能为素数;\t",a,a,m,a,m,m,m);
				printf("%.2f%c\n",p*100,'%');

				kx++;
			}
			else
			{
				printf("%d为合数\n",m);
				return 0;
			}
			
		}
		else{
			printf("%d为合数\n",m);
			return 0 ;
		}
	}

	fclose(fp);
	mirexit();
	getchar();
	return 0;
}
