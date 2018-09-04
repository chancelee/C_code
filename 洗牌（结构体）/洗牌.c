#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void change();

int main()
{
	struct car
	{
		char type;
		char c; 
	}card[54];
//////////////////////////
	int i,r;
	struct car *p=card;
	
	char poker[]={"23456789xJQKA"};
	
////////////////////////////////////
    for(i=0;i<52;i++)
	{
		card[i].type=i/13+3;
		card[i].c=poker[i%13+2];
/*		if(i%13==9) card[i].c='J';
		else if(i%13==10) card[i].c='Q';
		else if(i%13==11) card[i].c='K';
		else if(i%13==12) card[i].c='A';        */
	}

	card[52].type=0;
	card[53].type=0;
	card[52].c='K';
	card[53].c='k';	

/*	card[8].c='x';            //there is no ASCII of '10'
	card[21].c='x';
	card[34].c='x';
	card[47].c='x';     */

/*
		for(i=0;i<54;i++)
	{
		printf("%d     %d     %c  \n",i+1,card[i].type,card[i].c);
	}             */
/////////////////////////////////////////
	srand(time(0));
	printf("\tplayer1\t\tplayer2\t\tplayer3\n");
	for(i=0;i<54;i++)
	{
		if(i%3==0)
		{
			printf("\n\t");
		}
		r=rand()%(54-i)+i;
		change(p+i,p+r);
		printf("%d  %c\t\t",card[i].type,card[i].c);
	}
	printf("\n");
///////////////////////////////////////////////////
	return 0;
}
///////////////////////////////////////////////
void change(int *p1,int *p2)
{
	int temp;
	temp=*p1;
	*p1=*p2;
	*p2=temp; 
}





