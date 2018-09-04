#include<stdio.h>
main()
{
	char cipher[1000]="0",message[1000]="0";
	int k1,k2,k3;
	int i;
	int k;
	printf("input k1  k2:");
	scanf("%d %d",&k1,&k2);
	printf("input the cipher:\n");
    getchar();
   	gets(cipher);
	for(k=0;k<100;k++)
	{
		if(k1*k%26==1)
			k3=k;
	}
	for(i=0;i<strlen(cipher);i++)
	{
		
	    if(cipher[i]>='A'&&cipher[i]<='Z')
		{
			message[i]=(k3*(cipher[i]-65-k2)+26000)%26+65;
		}
	    else if(cipher[i]>='a'&&cipher[i]<='z')
		{	
		    message[i]=(k3*(cipher[i]-97-k2)+26000)%26+97;
		}
	    else
			message[i]=cipher[i];
	}
	printf("%s\n",message);
}