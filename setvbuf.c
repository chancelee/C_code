#include <stdio.h>
#include <windows.h>
int main()
{

   char buff[1024];

   memset( buff, '\0', sizeof( buff ));

   fprintf(stdout, "����ȫ����\n");
   setvbuf(stdout, buff, _IOFBF, 1024);

   fprintf(stdout, "������ runoob.com\n");
   fprintf(stdout, "����������浽 buff\n");
   printf("%s\n",buff);

   fprintf(stdout, "�⽫�ڱ��ʱ����\n");
   fprintf(stdout, "�������3����\n");
   
   fflush( stdout );
   printf("%s\n",buff);
   
   Sleep(3000);

   return(0);
}
