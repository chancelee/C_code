#include<stdio.h>
#include<math.h>
void output();
int main()
{
	FILE *fp;
	int member;
	int total;
	char *templet[]={"xinghangning","19971203","0517","4419","security"};
	char **p=templet;

	fp=fopen("password.txt","w");
	printf("input the members concerned of the password:");
	scanf("%d",&member); 

	total=pow(5,member);
	output(p,total,member,fp);
	fclose(fp);
	return 0;
}
void output(char **p,int total,int member,FILE *fp)
{
	int i;
	int n,j;
	for(i=0;i<total;i++)
	{
		n=i;
		j=0;
		while(j<member)
		{
			fprintf(fp,"%s",*(p+n%5));
			n=n/5;
			j++;
		}
		fprintf(fp,"    ");
	}
}