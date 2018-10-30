#include<stdio.h>
#include<time.h>
#include "miracl.h"
#include<math.h>

int main()
{
	miracl *mip = mirsys(90000,10);
	//功能说明: 初始化MIRACL系统，该函数必须在调用MIRACL库函数之前先执行	
	//初始化400位的10进制数

	int k,kx;
	FILE* fp = NULL;

	double p;
	//随机数种子为时间
	
	
	big bx = mirvar(0);
	big m = mirvar(0);
	big a = mirvar(0);
	big g = mirvar(0);
	big r = mirvar(0);
	big two = mirvar(2);
	big one = mirvar(1);
	big up_limit = mirvar(0);
	big m_1 = mirvar(0);
	//初始化bx=0
	char m_string[9999] = {0};
	char a_string[20] = {0};

	fp = fopen("data.txt","r");
	//printf("2.c\n");
	//printf("intput m(m>=3):");
	//scanf("%s",&m_string);
	fscanf(fp,"%s",m_string);
	cinstr(m,m_string);
	printf("m=:");
	cotnum(m,stdout);
	
	//printf("intput k:");
	//scanf("%d",&k);
	//getchar();
	fscanf(fp,"%d",&k);
	cotnum(k,stdout);
	
	decr(m,4,up_limit);
	// up_limit = m-4;

	kx = 1;
	//用时间戳设置随机数种子
	irand(time(0));
	for(;  kx <= k ;)
	{
		bigrand(up_limit,a);
		//产生一个随机数a , a<up_limit , 也就是a<m-4
		//后面还要+2 

		add(a,two,a);
		//a=a+2
		
		egcd(a, m, g);
		//g=gcd(a,m)
		
		if(compare(g, one)==0)//g==1
		{
			
			decr(m,1,m_1);
			//m_1 = m-1;
			powmod(a,m_1,m,r);
			// r= (a**m_1)%m
			//expint(a,m-1,bx);
				//bx = a**(m-1)
			//r = subdiv(bx,m,bx);
				//r = bx % m
			
			if(compare(r, one)==0)//r==1
			{
				//printf("比较r,1\t");
				cotstr(a,a_string);
				p = 1-(1/pow(2,kx));
				
				printf("a=%s, (%s,%s)=1,%s^(%s-1)(mod %s)≡1，故m=%d可能为素数;\t",a_string,a_string,m_string,a_string,m_string,m_string,m_string);
				
				printf("%.2f%c\n",p*100,'%');

				kx++;
			}
			else
			{
				printf("%d为合数\n",m);
				getchar();
				return 0;
			}
			
		}
		else{
			printf("%d为合数\n",m);
			getchar();
			return 0 ;
		}
	}

	fclose(fp);
	mirexit();
	getchar();
	return 0;
}
