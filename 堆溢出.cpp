#include<stdio.h>
#include<windows.h> 

char FILENAME[]="outfile";

int main(int argc,char* argv[])
{
	FILE * fd;
	long diff;
	char buff[100];	
	
	char *buf1=(char*)malloc(20);
	char *buf2=(char*)malloc(20);
	
	diff=long(buf2)-long(buf1);
 
 	strcpy(buf2,FILENAME);
	
	printf("info:\n");
	printf("buf1 address:%p\n",buf1);
	printf("buf2 address:%p,  with content:%s\n",buf2,buf2);
	printf("dis of two addresses:%d bytes\n",diff);
	printf("info end\n");
	
	if(argc<2)
	{
		printf("input the content to write in buf2:%s\n",buf2);	
		gets(buff);
		strcpy(buf1,buff);
	}
	else
	{
		printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");	
	}
	
	free(buf1);
	free(buf2); 
	 
	return 0;	
}
