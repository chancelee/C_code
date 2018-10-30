#include<stdio.h>
#include "miracl.h"
#include<math.h>
#include<time.h>
int main()
{
	int m,k,kx;
	int a;
	int g;
	int r;
	double p;
	miracl *mip = mirsys(500,10);
	FILE* fp = fopen("data.txt","r");
	
	big bx = mirvar(0);

	srand(time(0));
	//printf("2.c\n");
	//printf("intput m(m>=3):");
	fscanf(fp,"%d",&m);
	//scanf("%d",&m);
	//getchar();
	printf("m=:%d\n",m);

	//printf("intput k:");
	fscanf(fp,"%d",&k);
	//scanf("%d",&k);
	//getchar();
	printf("k=:%d\n",k);

	kx = 0;

	a = 2;
	//for(a=2;(a<=m-2)&&(kx<=k);a++)
	while((a>=2) && (a<=m-2) && (kx<k))
	{
		g = igcd(a,m);
		if(g == 1)
		{
			expint(a,m-1,bx);
			r = subdiv(bx,m,bx);
			
			if(r == 1)
			{
				
				p = 1-(1/pow(2,kx));
				printf("a=%d, (%d,%d)=1,%d^(%d-1)(mod %d)≡1，故m=%d可能为素数;\t",a,a,m,a,m,m,m);
				printf("%.2f%c\n",p*100,'%');

				kx++;
			}
			
		}

		a = 2 + rand()%(k);
	}

	mirexit();
	fclose(fp);
	getchar();
	return 0;
}
