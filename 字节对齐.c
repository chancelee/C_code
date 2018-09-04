#include<stdio.h>
struct A{
    int    a;
    char   b;
    short  c;
};
struct B{
    char   b;
    int    a;
    short  c;
};
int main(int argc,char *argv[],char **env)
{
	printf("A:%d\nB:%d\n",sizeof(struct A),sizeof(struct B)); 
	
	return 0;
}
