#include<stdio.h>
#include<string.h>
void main()
{
	char message[1000]="0",cipher[1000]="0";
	int k1,k2;
	int i;
	printf("input k1 k2:");
	scanf("%d %d",&k1,&k2);
	printf("input the message:\n");
	getchar(); 
	gets(message);
	for(i=0;i<strlen(message);i++)
	{
		if(message[i]>='A'&&message[i]<='Z')
			{
			cipher[i]=((message[i]-65)*k1+k2)%26+65;
			}
	    else if(message[i]>='a'&&message[i]<='z')
			{
		    cipher[i]=((message[i]-97)*k1+k2)%26+97;
			}
		else 
			cipher[i]=message[i];  //deal with the blank
	}
	puts(cipher);
}