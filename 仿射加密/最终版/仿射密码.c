#include<stdio.h>
#include<string.h>
void encrypt(int a,int b);
void decrypt(int k2,int k3);
void frequency();
void brute();
int max(int number1,int number2);
int min(int number1,int number2);
char message[1000]="0";
char cipher[1000]="0";
int main()
{

	int c;
	int k1,k2,k3;
	printf("input a number to choose the mode:\n");
	printf("1.encrypt   2.decrypt   3.decrypt knowing the secret keys   4.decrypt via the frequency of letters\n");
	scanf("%d",&c);
	switch(c)
	{
	case 1 :
		{
         	printf("input k1 k2:");
	        scanf("%d %d",&k1,&k2);
        	printf("input the message:\n");
	        getchar();
        	gets(message);
			encrypt(k1,k2);
			break;
		}
	case 2 :
		{
			printf("input k1  k2:");
			scanf("%d %d",&k1,&k2);
			printf("input the cipher:\n");
		    getchar();
        	gets(cipher);
			decrypt(k1,k2);
			break;
		}
	case 3 :
		{
			brute();
			break;
		}
	case 4:
		{
			getchar();
			frequency();
			break;
		}
	default:printf("enter data error!\n");
	}
	
	getchar();
	return 0; 
}

void encrypt(int k1,int k2)
{
	unsigned int i;
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
void decrypt(int k1,int k2)
{
	unsigned int i;
	int k,k3;
	for(i=0;i<strlen(cipher);i++)
	{
		for(k=0;k<100;k++)
		{
	    if(k1*k%26==1)
	    	k3=k;
		}
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
	puts(message);
}
void brute()
{
	int k1,k2,k3,n;
	char word[100]="0";
	getchar();     
	printf("input the cipher:\n");
	gets(cipher);
	printf("\ninput the symbolic words:\n");
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
				else if(cipher[n]>='a'&&cipher[n]<='z')
					message[n]=(k3*(cipher[n]-'a'-k2)+26000)%26+'a';
				else message[n]=cipher[n];
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
void frequency()
{
	int number[26]={0};
	int i,a,b,k;
	int k1,k2,k3;
	int n=0,m=1;
	char ln,lm;
	printf("input the cipher:");
    gets(cipher);
	for(a=65;a<=90;a++)
	{
		b=a+32;
		k=a-65;
		for(i=0;i<strlen(cipher);i++)
		{
			if(cipher[i]==a||cipher[i]==b)
				number[k]=number[k]+1;
		}
	}
	for(k=0;k<26;k++)
	{
		printf("the letter:%c    the number:%d\n",k+65,number[k]);
		n=max(n,number[k]);
		m=min(m,number[k]);
	}      
	for(k=0;k<26;k++)
	{
		if(number[k]==n)
		{
			printf("the most frequent letter:%c     %d\n",k+65,n);
			ln=k+97;
		}
		else if(number[k]==m)
		{
			printf("the less frequent letter:%c     %d\n",k+65,m);
			lm=k+97;
		}
	}               //ensure the most and less frequent letter
		for(k1=0;k1<26;k1++)
		{

		     for(k2=0;k2<26;k2++)
			 {
				 if((ln==(('e'-97)*k1+k2+260000)%26+97)&&(lm==(('z'-97)*k1+k2+260000)%26+97))
				 {
					 printf("k1:%d     k2:%d\n",k1,k2);
					 goto out;
				 }
			 }
		}out:                    //ensure k1 and k2
		for(k=0;k<100;k++)
		{
			if(k1*k%26==1)
				k3=k;
		}                        //ensure k3
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
	    printf("k3:%d\n",k3);
	    puts(message);
}     
int max(int number1,int number2)
{
	int max=number1;
	if(number1<number2)
		max=number2;
	return max;
}
int min(int number1,int number2)
{
	int min=number1;
	if(number1>number2)
		min=number2;
	return min;
}
