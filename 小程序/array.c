#include<stdio.h>
int main()
{
	int T;
	int n[10]={0};
	int a[10][100]={0};
	
	int i,j;
	int delta;
	
	//printf("intput T:");
	scanf("%d",&T);
	getchar();//���ܻس��ո� 
	
	for(i=0;i<T;i++)//T������ 
	{
		//printf("input n:");
		scanf("%d",&n[i]);
		getchar();
		for(j=0;j<n[i];j++) // n������ 
		{
			scanf("%d",&a[i][j]);
			getchar();
		}
	}

	for(i=0;i<T;i++)
	{
		delta = a[i][1] - a[i][0];
		for(j=1;j<n[i];j++)
		{
			if(delta!=(a[i][j] - a[i][j-1]) )
			{
				printf("no\n",i);
				break;
			}
			
			//ÿ���м�û��break��˵���Ȳ� 
			if(j==n[i]-1) 
			printf("%d\n",i,delta);
		}	
	}
	return 0;
}
