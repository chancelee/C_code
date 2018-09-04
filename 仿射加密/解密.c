#include<stdio.h>
main()
{
	int i;
	char cipher;
	while((cipher=getchar())!='\n')
	{
		if(cipher>='A'&&cipher<='Z')
		{
			cipher=cipher-65;
			i=(9*(cipher-5)+26000)%26+65;
		}
		else
		{
			cipher=cipher-97;
			i=(9*(cipher-5)+26000)%26+97;
		}
		printf("%c",i);
	}
	printf("\n");
}