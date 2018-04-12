#include "syscall.h"
#include "../include/string.h"
#include "../include/printf.h"
#include "dirent.h"
#include "sysdef.h"
#include "stdio.h"
#include "../include/unistd.h"

void  ls()
{
	int n=0;
	//struct linux_dirent *d;
	//sb_pwd();

    /*char* firstToken=strtok(1,pwd,'/');

    if (!_strcmp(firstToken,"rootfs",6)){
        if (*(pwd)=='/'){
            *pwd=*pwd+8;
        }
        else{
            *pwd=*pwd+7;
        }
    }
    put_s(pwd);*/

    char *path="bin/";

    //put_s(path);

	n=opendir(path);
    printf("%d in between",n);
    n=readdir(n);
    /*int i=0;
    for (i = 0;i < n;) {
        d = (struct linux_dirent *) (direc + i);
        put_s(d->fd_name);
        put_s("\t");
        i += d->fd_reclen;
    }*/
    put_s("\n");
}

