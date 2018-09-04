#include<stdio.h>
int max(int number1,int number2);
int min(int number1,int number2);
main()
{
	char cipher[1000]="0",message[1000]="0";
	int number[26]={0};
	int i,a,b,k;
	int k1,k2,k3;
	int n=0,m=1;
	char ln,lm;     //ln: the most frequent letter  lm:the less frequent letter
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