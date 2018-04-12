#include "syscall.h"
#include "string.h"
#include "dirent.h"
#include "sysdef.h"
#include "../include/printf.h"
#include "stdio.h"
#include "unistd.h"

char* shell(char *inp) {
    int n=0;
    _strcpy_(inp,temp);
    put_s(inp);
    int mode=0;
    n=_open_(0x05,temp,mode);
    printf("%d abcd",n);
    n=_read_(3 ,(long)n,buf,1000);
    return buf;
    //put_s(buf);
    //put_s("\n");
}