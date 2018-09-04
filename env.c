#include<stdio.h>
#include<stdlib.h> 
int main(int argc,char* argv[],char **env)
{
	//printf("%s\n",*(env+2)); 
	int i;
	for(i=0;i<10;i++)
	{
		printf("%s\n",*(env+i));	
	}
	return 0;
}
