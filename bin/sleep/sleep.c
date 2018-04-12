#include "syscall.h"
#include "string.h"
#include "dirent.h"
#include "unistd.h"
#include "sysdef.h"
#include "stdio.h"
#include "printf.h"

void sleep(int sec){
    _sleep_(162,sec);
    printf("\nAfter %d seconds\n",sec);
}

int main(int argc,char*argv[]){
    char * seconds=sb_malloc(10* sizeof(char));

    //int sec=seconds[0]-48;
    _strcpy_(argv[1],seconds);
    char i=seconds[0];
    int sec=0;
    int k=0;
    int m=1;
    int j=0;
    int in=1;
    while (1){
        while (j<_strlen_(argv[1])-in){
            m=m*10;
            j++;
        }
        k=(i-48)*m;
        sec=sec+k;
        if (in==_strlen_(argv[1])){
            break;
        }
        i=argv[1][in];
        if (i=='\0'){
            break;
        }
        in++;
        m=1;
        j=0;
    }
    //printf("\n-------%d-----\n",sec);
    sleep(sec);
    return 0;
}