// ensure the password includes capital/low letters and numbers at all

#include<stdio.h>


void initial_templet();
void initial_password();
void letter();


int n;

char password[30]={0};
char templet[63]={0};

FILE *fp;

int main()
{

	int i=0;
	char *temp;
 	char *p;  //->password that is used to print

	temp=templet;
	p=password;

	if((fp=fopen("password.txt","w"))==NULL)
	{
		printf("error\n");
		exit(0);
	}
	

	printf("input the length of the password:");
	scanf("%d",&n);          //n:the length of the password

	initial_templet(temp);
	initial_password(p);
//	printf("%s\n",password);
	
	letter(i);
	
	fclose(fp);

	return 0;
}


void initial_templet(char *temp)
{
	int i;
	for(i=0;i<62;)
	{
		if(i==0)
		{
			for(;i<10;i++)
			{
				*(temp+i)=i+'0';
			}
		}
		else if(i==10)
		{
			for(;i<36;i++)
			{
				*(temp+i)=i-10+'A';
			}
		}
		else if(i==36)
		{
		    for(;i<62;i++)
			{
				*(temp+i)=i-36+'a';
			}
		}
	}
}


void initial_password(char *p)
{
	int i;
	for(i=0;i<n;i++)
	{
		*(p+i)=templet[0];
	}
}


void letter(int i)
{
	int j;
	if(i==n-1)
	{
		for(j=0;j<62;j++)
		{
		//	printf("%s\n",password);
			fputs(password,fp);
			fputs("      ",fp);
		//	fputc('0',fp);
		//	fputc('\t',fp);
			password[n-1]=templet[j+1];     
		}
		password[n-1]=templet[0];
	}
	else
	{
		for(j=0;j<62;j++)
		{
			letter(i+1);
			password[i]=templet[j+1];
		}
		password[i]=templet[0];
	}
}            
