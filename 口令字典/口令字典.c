// ensure the password includes capital/low letters and numbers at all
#include<stdio.h>
#include<math.h>

void letter();
int main()
{
	FILE *fp;
	int length;
	int total;
	char templet[63]={"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
	char *temp;

	temp=templet;
	fp=fopen("password.txt","w");

	printf("input the length of the password:");
	scanf("%d",&length);   
	

	total=pow(62,length);
	letter(temp,total,length,fp);

	fclose(fp);
	return 0;
}
void letter(char *temp,int total,int length,FILE *fp)
{
	int i;
	int n,j;
	for(i=0;i<total;i++)
	{
		n=i;
		j=0;
		while(j<length)
		{
			fprintf(fp,"%c",*(temp+n%62));
			n=n/62;
			j++;
		}
		fprintf(fp,"    ");
	}
}





