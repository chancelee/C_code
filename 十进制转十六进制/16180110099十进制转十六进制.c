#include<stdio.h>
#include<string.h>
int value(char s[8],int n);
void swap(char s[8],int i);
void output(char s[8],int i);
int main()
{
	int n,i;
	char s[8]="0";
	printf("input the decimal number:");
	scanf("%d",&n);  
	if(n<0)
	{
	}
	i=value(s,n);
	swap(s,i); 
	output(s,i);

	printf("\n");
	return 0;
}
////////////////////////////////////////////////////////
int value(char s[8],int n)
{
	int i=0;
	while(n)
	{
		s[i++]=n%16;
		n=n/16;	
	}
	return i;            //i:the elements' number of s[]
}
//////////////////////////////////////////////////////////
void swap(char s[8],int i)
{
	int j,k,temp;
	for(k=0,j=i-1;k<i/2;k++,j--)
	{
		temp=s[k];
		s[k]=s[j];
		s[j]=temp;
	} 
}
//////////////////////////////////////////
void output(char s[8],int i)
{	
	int k;
	for(k=0;k<i;k++)
	{
		if(s[k]>9)
			printf("%c",s[k]-10+'A');
		else
			printf("%d",s[k]);
	}
}