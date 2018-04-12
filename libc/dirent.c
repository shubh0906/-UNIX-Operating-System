#include "dirent.h"
#include "sysdef.h"
#include "syscall.h"
#include "string.h"

long readdir(long read)
{
  return _getdents_(0xdc,read,direc,1000);  
}

int opendir(char *dir)
{
  _strcpy_(dir,direc);
  return _open_(0x05,direc,4);
}
