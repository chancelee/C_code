#define  _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"

int main() {

    //如果加密数据太大,需要进行拆分
    unsigned char *inData = "1234567890987654321";
    unsigned char outData[4096] = {0};
    int outLen = 0;
    //des加密
    int ret = DesEnc(inData,strlen(inData),outData,&outLen);

    printf("原码 inData=%s\n",inData);
    printf("加密后 outData=%s\n",outData);

    unsigned char outD2[4096] = {0};
    int len2;
    //des解密
    int ret2 = DesDec(outData,outLen,outD2,&len2);
    printf("解密 outD2=%s\n",outD2);

    getchar();
    return 0;
}