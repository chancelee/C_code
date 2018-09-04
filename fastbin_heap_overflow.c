#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    void *buf0, *buf1, *buf2;
    buf0 = malloc(32);
    buf1 = malloc(32);

    free(buf1);
    free(buf0);

    buf0 = malloc(32);
    read(0, buf0, 64);
    /*
	该代码在调用read时，向buf0写入的内容超过了其本身的大小，
	发生了堆溢出。我们可以利用此处漏洞，修改相邻chunk的fd内容，
	造成在为buf2分配内存时，从fastbin返回得到非正常的地址。
    */
	buf1 = malloc(32);

    buf2 = malloc(32);

    printf("buf2 is at %p\n", buf2);

    return 0;
}

