    #include <stdio.h>  
    #include <stdlib.h>  
      
    void myprintf(char* fmt, ...) //一个简单的类似于printf的实现，//参数必须都是int 类型  
    {  
        char* pArg=NULL; //等价于printf原始实现的va_list  
        char c;  
        pArg = (char*) &fmt; //注意不要写成p = fmt !!因为这里要对//参数取址，而不是取值  
         pArg += sizeof(fmt); //等价于原来的va_start  
      
        do  
        {  
            c =*fmt;  
            if (c != '%')  
            {  
                putchar(c); //照原样输出字符  
              }  
            else  
            {  
                //按格式字符输出数据  
                   switch(*++fmt)  
                {  
                    case 'd':  
                        printf("%d",*((int*)pArg));  
                        break;  
                    case 'x':  
                        printf("%#x",*((int*)pArg));  
                        break;  
                    default:  
                        break;  
                }  
                pArg += sizeof(int); //等价于原来的va_arg  
            }  
            ++fmt;  
        }while (*fmt != '\0');  
        pArg = NULL; //等价于va_end  
        return;  
    }  
      
    int main(int argc, char* argv[])  
    {  
        int i = 1;  
        int j = 2;  
        myprintf("the first test:i=%d\n",i,j);  
        myprintf("the secend test:i=%d; %x;j=%d;\n",i,0xabcd,j);   
        return 0;  
    }  
