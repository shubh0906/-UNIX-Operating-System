#include "wait.h"
#include "syscall.h"

int  sb_waitpid(long pid,int *status,long options)
{
  return sys_waitpid(7,pid,status,options);
}
