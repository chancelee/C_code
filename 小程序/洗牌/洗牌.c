#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void change();
main()
{
	int i,r,temp;
	int pk[54];
	int *p=pk;
	for(i=0;i<54;i++)
	{
		pk[i]=100*(i/13+1)+i%13+1;
	}
	srand(time(0));
	for(i=0;i<53;i++)
	{
		if(i%13==0)
			printf("\nplayer  ");
		r=rand()%(54-i)+i;
		change(p+i,p+r);
/*		temp=pk[i];
		pk[i]=pk[r];
		pk[r]=temp;
		printf("%d  ",pk[i]); */       
	}
	printf("%d\n",pk[i]);
}
void change(int *p1,int *p2)
{
	int temp;
	temp=*p1;
	*p1=*p2;
	*p2=temp;
	printf("%d  ",*p1);  
}
