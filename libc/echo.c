#include "syscall.h"
#include "../include/string.h"
#include "../include/printf.h"
#include "dirent.h"
#include "sysdef.h"
#include "stdio.h"
#include "../include/unistd.h"

void echo(char* value){
    //strtok
    printf("%s",value);
}