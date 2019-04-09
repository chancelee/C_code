#include<stdio.h>
void p();
int main()
{
	void (*pfunc)(int) = NULL;
	pfunc = p;
	(*)pfunc(1);
	return 0;
}

void p(int a)
{
	printf("%d %d\n",a,a);
}
