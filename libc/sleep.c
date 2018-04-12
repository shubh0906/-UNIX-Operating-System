#include "syscall.h"
#include "string.h"
#include "dirent.h"
#include "unistd.h"
#include "sysdef.h"
#include "stdio.h"
#include "printf.h"

void sleep(int sec){
    _sleep_(162,sec);
}

/*int main(int argc,char*argv[]){
    char * seconds=sb_malloc(10* sizeof(char));
    int sec=seconds[0]-48;
    sleep(sec);
    return 0;
}*/