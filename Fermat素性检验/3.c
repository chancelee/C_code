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
	//���������Ϊʱ��
	miracl *mip = mirsys(40000,10);
	//����˵��: ��ʼ��MIRACLϵͳ���ú��������ڵ���MIRACL�⺯��֮ǰ��ִ��	
	//��ʼ��400λ��10������
	
	big bx = mirvar(0);
	//��ʼ��bx=0

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
	//��ʱ����������������
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
				
				printf("a=%d, (%d,%d)=1,%d(%d-1)(mod %d)��1����m=%d����Ϊ����;\t",a,a,m,a,m,m,m);
				printf("%.2f%c\n",p*100,'%');

				kx++;
			}
			else
			{
				printf("%dΪ����\n",m);
				return 0;
			}
			
		}
		else{
			printf("%dΪ����\n",m);
			return 0 ;
		}
	}

	fclose(fp);
	mirexit();
	getchar();
	return 0;
}
