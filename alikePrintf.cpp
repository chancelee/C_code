    #include <stdio.h>  
    #include <stdlib.h>  
      
    void myprintf(char* fmt, ...) //һ���򵥵�������printf��ʵ�֣�//�������붼��int ����  
    {  
        char* pArg=NULL; //�ȼ���printfԭʼʵ�ֵ�va_list  
        char c;  
        pArg = (char*) &fmt; //ע�ⲻҪд��p = fmt !!��Ϊ����Ҫ��//����ȡַ��������ȡֵ  
         pArg += sizeof(fmt); //�ȼ���ԭ����va_start  
      
        do  
        {  
            c =*fmt;  
            if (c != '%')  
            {  
                putchar(c); //��ԭ������ַ�  
              }  
            else  
            {  
                //����ʽ�ַ��������  
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
                pArg += sizeof(int); //�ȼ���ԭ����va_arg  
            }  
            ++fmt;  
        }while (*fmt != '\0');  
        pArg = NULL; //�ȼ���va_end  
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