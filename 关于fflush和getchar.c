#include<stdio.h>
int main()
{
	char a = 0;
	char b = 0;
	char c = 0;


	printf("input a,b:");
	scanf("%c%c",&a,&b);
	//fflush(stdin);
	c = getchar();
	printf("a:%c\n",a);
	printf("b:%c\n",b);
	printf("c:%c\n",c);
	
	printf("input b:");
	scanf("%c",&b);
	printf("b:%c\n",b);
	
	
	return 0;
}
