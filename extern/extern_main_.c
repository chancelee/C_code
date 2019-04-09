#include<stdio.h>
#include "extern.c"
extern int a; 
int main()
{
	a=1;
	printf("%p\n",&a);
	printf("%d\n",a);	
	
	return 0;
}
