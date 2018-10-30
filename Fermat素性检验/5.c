#include<stdio.h>
#include<time.h>
#include "miracl.h"
#include<math.h>

int main()
{
	miracl *mip = mirsys(90000,10);
	//����˵��: ��ʼ��MIRACLϵͳ���ú��������ڵ���MIRACL�⺯��֮ǰ��ִ��	
	//��ʼ��400λ��10������

	int k,kx;
	FILE* fp = NULL;

	double p;
	//���������Ϊʱ��
	
	
	big bx = mirvar(0);
	big m = mirvar(0);
	big a = mirvar(0);
	big g = mirvar(0);
	big r = mirvar(0);
	big two = mirvar(2);
	big one = mirvar(1);
	big up_limit = mirvar(0);
	big m_1 = mirvar(0);
	//��ʼ��bx=0
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
	//��ʱ����������������
	irand(time(0));
	for(;  kx <= k ;)
	{
		bigrand(up_limit,a);
		//����һ�������a , a<up_limit , Ҳ����a<m-4
		//���滹Ҫ+2 

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
				//printf("�Ƚ�r,1\t");
				cotstr(a,a_string);
				p = 1-(1/pow(2,kx));
				
				printf("a=%s, (%s,%s)=1,%s^(%s-1)(mod %s)��1����m=%d����Ϊ����;\t",a_string,a_string,m_string,a_string,m_string,m_string,m_string);
				
				printf("%.2f%c\n",p*100,'%');

				kx++;
			}
			else
			{
				printf("%dΪ����\n",m);
				getchar();
				return 0;
			}
			
		}
		else{
			printf("%dΪ����\n",m);
			getchar();
			return 0 ;
		}
	}

	fclose(fp);
	mirexit();
	getchar();
	return 0;
}
