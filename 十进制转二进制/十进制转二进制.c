#include<stdio.h>
int main()
{
	int i=0;
	int x;
	printf("input a number:");
	
	scanf("%d",&x);
	while(x!=0)
	{
	while(i<32)
	{
		printf("%d",(x&0x80000000)?1:0);
		x=x<<1;
		i++;
	}
	printf("\n");
	scanf("%d",&x);
	}
	printf("\n");
	return 0;
}