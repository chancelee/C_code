#include<stdio.h>
#include<string.h>
main()
{
	int k1,k2,k3,n;
	char cipher[1000]="0",message[1000]="0";
	char word[100]="0";
	gets(cipher);
	gets(word);
	for(k1=0;k1<26;k1++)
	{
		for(k2=0;k2<26;k2++)
		{
			for(k3=0;k3<1000;k3++)
			{if(k3*k1%26==1) break;}
			for(n=0;n<strlen(cipher);n++)
			{
				if(cipher[n]>='A'&&cipher[n]<='Z')
					message[n]=(k3*(cipher[n]-'A'-k2)+26000)%26+'A';
				else message[n]=(k3*(cipher[n]-'a'-k2)+26000)%26+'a';
				if(cipher[n]-k2<0) message[n]=message[n]+26;
			}
			if (strstr(message,word))
			{
				printf("\n{the message:%s   }\n",message);
				printf("secret key:%d    %d\n",k1,k2);
			}
			else printf("%s    ",message);
		}
	}
}