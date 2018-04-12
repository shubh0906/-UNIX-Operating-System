#ifndef _SYSCALL_H
#define _SYSCALL_H


 extern long _execve_ (long syscall, char* fd, char* str[], char* count[]);
 extern long _getdents_(long syscall, long fd, char  *buf ,long count);

 
 extern long _dup2_(long syscall, long ofd, long nfd);

 extern long _pipe_(long syscall, long fd);

 extern long _sleep_(long syscall,long time);

extern long _close_(long syscall,long fd);
 extern long _write_(long syscall, long fd, long str, long count);

 extern long _read_ (long syscall, long fd, char* str, long count);

extern long _malloc_(long syscall, long size);

extern long _clearscreen_(long syscall);

 extern long _open_(long syscall, char* str, long count );

 extern long _close_(long syscall,long fd );

 extern long _exit_(long syscall,int code);

 extern long _chdir_(long syscall,char* path);

 extern long _pwd_(long syscall,char* path,long size);

 extern long _fork_(long syscall);

 extern long sys_waitpid(long syscall, long pid,int *status  ,long options );

 extern  long _yield_(long syscall);

#endif

