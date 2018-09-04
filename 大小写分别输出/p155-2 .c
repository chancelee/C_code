#include<stdio.h>
#include<string.h>
void u(char letter[100]);
void l(char letter[100]);
main()
{
	char letter[100]="0";
	printf("input the words:\n");
	gets(letter);
	u(letter);
	l(letter);
}
void u(char letter[100])
{
	int i;
	for(i=0;i<strlen(letter);i++)
	{
		if(letter[i]>=65&&letter[i]<=90)
			printf("%c",letter[i]);	
	}
	printf("\n");
}
void l(char letter[100])
{
	int i;
	for(i=0;i<strlen(letter);i++)
	{
		if(letter[i]>=97&&letter[i]<=122)
			printf("%c",letter[i]);
	}
	printf("\n");
}