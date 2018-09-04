#include<stdio.h>
#include<string.h>
void count(char word[100]);
main()
{
	char word[100]="0";
	printf("input the words:\n");
	gets(word);
	count(word);
}
void count(char word[100])
{
	int i;
	int l=0,n=0,b=0,c=0;     //l:letter  n:number   b:blank    c:character
	for(i=0;i<strlen(word);i++)
	{
		if ((word[i]>='A'&&word[i]<='Z')||(word[i]>='a'&word[i]<='z'))
			l++;
		else if (word[i]>=48&&word[i]<=57)
			n++;
		else if(word[i]==' ')
			b++;
		else
			c++;
	}
	printf("letter:%d\nnumber:%d\nblank:%d\ncharacter:%d\n",l,n,b,c);
}