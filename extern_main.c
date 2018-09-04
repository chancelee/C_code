#include<stdio.h>
#include<string.h>
#include "extern.c"
extern int a; 
int main(int argc, char *argv[])
{

	printf("%p\n",&a);
	return 0;
}
