#include<stdio.h>
#include "miracl.h"
#include<math.h>
int main()
{
	int m,k,kx;
	int a;
	int g;
	double p;
	miracl *mip = mirsys(400,10);
	big br = mirvar(0);
	big ba = mirvar(0);	
	big bx = mirvar(0);
	big bm = mirvar(277);

	printf("intput m(m>=3):");
	scanf("%d",&m);
	getchar();
	printf("m=:%d\n",m);

	printf("intput k:");
	scanf("%d",&k);
	getchar();
	printf("k=:%d\n",k);

	kx = 1;
	for(a=2;(a<=m-2)&&(kx<=k);a++)
	{
		convert(a, ba);
		g = igcd(a,277);
		if(g == 1)
		{
			decr(bm,1,bx);
			powmod(ba, bx, bm, br);

			convert(1,bx);
			if(!(compare(br, bx)))
			{
				
				p = 1-(1/pow(2,kx));
				printf("a=%d, (%d,%d)=1,%d(%d-1)(mod %d)≡1，故m=%d可能为素数;\t",a,a,m,a,m,m,m);
				printf("%.2f%c\n",p*100,'%');

				kx++;
			}
			else
			{
				printf("false");
			}
			
		}
	}
	
	mirkill(bm);
	mirkill(br);
	mirkill(ba);
	mirkill(bx);
//	mirkill(p);

	getchar();
	return 0;
}
