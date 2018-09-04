#include<stdio.h>
#include<stdlib.h>

int main()
{
	char *p1=(char*)malloc(sizeof(char)*10);
	memcpy(p1,"hello",10);
	printf("p1 address:%p\nvalue:%s\n",p1,p1);
	free(p1);
	
	char *p2=(char*)malloc(sizeof(char)*10);
	memcpy(p2,"world",10);
	printf("p2 address:%p\np1 value:%s\n",p2,p1);
	free(p2);
	
	return 0;	
}
