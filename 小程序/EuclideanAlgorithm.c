//applied to affine cipher

#include<stdio.h>

int main()
{
	//ax mod 26 == 1
	//ax + by = 1
	int a,b;
	int x1,x2,x3,y1,y2,y3;
	int temp1,temp2,temp3;
	int q;

	printf("input a:");
	scanf("%d",&a);
	fflush(stdin);
	printf("input b:");
	scanf("%d",&b);
	fflush(stdin);
	

	x1 = 1;
	x2 = 0;
	x3 = a;
	y1 = 0;
	y2 = 1;
	y3 = b;
	printf("%d\n",11 / 26);
	printf("q	x1	x2	x3	y1	y2	y3\n");
	printf("---------------------------------------------------\n");
	printf("NONE	%3d	%3d	%3d	%3d	%3d	%3d\n",x1,x2,x3,y1,y2,y3);
	

	while(y3 != 0)
	{

		q = x3 / y3;
		temp1 = x1;
		temp2 = x2;
		temp3 = x3;
		x1 = y1;
		x2 = y2;
		x3 = y3;
		y1 = temp1 - q * y1;
		y2 = temp2 - q * y2;
		y3 = temp3 - q * y3;
		
		printf("%3d	%3d	%3d	%3d	%3d	%3d	%3d\n",q,x1,x2,x3,y1,y2,y3);
		
	}
	
	printf("%dx mod 26 == 1 \n",a);
	printf("%dx +%dy == 1 \n",a,b);
	printf("x == %d \n y==%d\n",x1,x2);
	printf("gcd(%d,%d) == %d\n",a,b,x3);
	
	getchar();
	return 0;
}
