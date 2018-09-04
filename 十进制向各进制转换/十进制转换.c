#include<stdio.h>
void trans_b(int x);      //transform to binary
void trans_o(int x);      //transform to octal
void trans_h(int x);      //transform to hexadecimal
int main()
{
	int m,x;
	printf("choose the radix to transform:\n1.binary\n2.octal\n3.hexadecimal\n");
	scanf("%d",&m);    
	printf("input the decimal number:");
	scanf("%d",&x);
	if(m==1)
	{
		trans_b(x);
	}
	else if(m==2)
	{
		trans_o(x);
	}
	else if(m==3)
	{
		trans_h(x);
	}
	return 0;
}
/////////////////////////////////////////////////
void trans_b(int x)
{
	int i=0;
	while(i<32)
	{
		printf("%d",(x&0x80000000)?1:0);
		x=x<<1;
		i++;
	}
	printf("\n");
}
////////////////////////////////////////
void trans_o(int x)
{
	int i;
	int a[11]={0};
	for(i=0;i<11;i++)
	{
		if(i==10)
			a[10]=x&3;
		
		else 
			a[i]=x&7;
	     	x=x>>3;
	}
	for(i=10;i>=0;i--)
	{
		printf("%d",a[i]);
	}
	printf("\n");
}
//////////////////////////////////////////////
void trans_h(int x)
{
	int i;
	int a[8]={0};
	for(i=0;i<8;i++)
	{
		a[i]=x&15;
		x=x>>4;
	}
	for(i=7;i>=0;i--)
	{
		if(a[i]<10)
			printf("%d",a[i]);
		else 
			printf("%c",a[i]-10+'A');
	}
	printf("\n");
}