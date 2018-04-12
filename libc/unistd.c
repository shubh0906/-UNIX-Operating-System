#include "unistd.h"
#include "syscall.h"
#include "sysdef.h"
#include  "stdio.h"
#include "printf.h"

long sb_chdir(char * str)
{
  int ret=0;
  ret=_chdir_(0x0c,str);
  return ret;
}

void sb_pwd(int print)
{
  _pwd_(0xb7,pwd,1000);
  if(print==1)
    printf("%s\n",pwd);
  

}

void sb_pipe(int fd)
{
  _pipe_(0x2a,fd);
}

void sb_dup2(int ofd,int nfd)
{
  _dup2_(0x3f,ofd,nfd);
}

long sb_fork()
{
  long ret=0l;
  ret=_fork_(2);
  //printf("out of fork");
  return ret;
}

void * sb_malloc(int size){
  long ret;
  ret=_malloc_(0x5A,size);
  return (void *)ret;
}

void sb_exit(int code)
{
  _exit_(1,code);
}

void sb_close(long fd)
{
  _close_(0x06,fd);
}
int sb_yield()
{
  return _yield_(0x9e);

}
int sb_execve(char *fd,char **argv,char **env)
{
//	put_s(fd);
//	put_s(argv[0]);
//	put_s(argv[1]);
//	//put_s(argv[2]);
//	put_s(env[11]);
	return  _execve_(11,fd,argv,env);
	
}
