#include <stdlib.h>
#include <stdio.h>

int cmp ( const void *a , const void *b ) 
{ 
  return *(int *)a - *(int *)b; 
} 

int main(int argc, char *argv[])
{
	int a[3] = {5,4,3};
	int i = 0;
	qsort(a,3,sizeof(a[0]),cmp);
	for(i = 0; i < 3; i++)
	{
		printf("%d\n",a[i]);
	}
	return 0;
}


 