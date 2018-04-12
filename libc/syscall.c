#include "sysdef.h"
#include "syscall.h"
#include "printf.h"

 inline long _execve_ (long syscall, char* fd, char* str[], char* count[]) {
     long ret;
     __asm__  __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (fd), "c" (str), "d" (count): "memory");
     return ret;
  }
 inline long _getdents_(long syscall, long fd, char  *buf ,long count) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (fd),  "c" (buf),"d" (count): "memory");
   return ret;
}
 inline long _dup2_(long syscall, long ofd, long nfd) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (ofd),  "c" (nfd): "memory");
   return ret;
}

inline long _malloc_(long syscall, long size) {
    long ret;
    __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (size): "memory");
    return ret;
}

inline long _clearscreen_(long syscall) {
    long ret;
    __asm__ __volatile__ ("int $0x80": "=a" (ret):"a" (syscall): "memory");
    return ret;
}

 inline long _pipe_(long syscall, long fd) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (fd): "memory");
   return ret;
}
 inline long _sleep_(long syscall,long time) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b"(time): "memory");
   return ret;
}

 inline long _write_(long syscall, long fd, long str, long count) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (fd), "c" (str), "d" (count): "memory");
   
   return ret;
}

 inline long _read_ (long syscall, long fd, char* str, long count) {
   volatile long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (fd), "c" (str), "d" (count): "memory","cc");
   return ret;
}

 inline long _open_(long syscall, char* str, long count ) {
   volatile long ret;
   //printf("in oppen");
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (str), "c" (count): "memory","cc"); //, "d" (0777)
   
   //printf("%d   --shubham----\n",ret);

   return ret;
}
 inline long _close_(long syscall,long fd ) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (fd): "memory","cc");

   return ret;
}
 inline long _exit_(long syscall,int code) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (code), "c" (0), "d" (0): "memory","cc");
   return ret;
}
 inline long _chdir_(long syscall,char* path) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (path): "memory","cc");
   return ret;
}
 inline long _pwd_(long syscall,char* path,long size) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (path), "c" (size): "memory","cc");
   return ret;
}
 inline long _fork_(long syscall) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall): "memory","cc");
   return ret;
}
inline long sys_waitpid(long syscall, long pid,int *status  ,long options ) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall),"b" (pid), "c" (status), "d" (options): "memory","cc");

   // /printf("waittttttttttttttttttttttt%d\n", ret);
   return ret;
}
inline long _yield_(long syscall) {
   long ret;
   __asm__ __volatile__ ("int $0x80": "=a" (ret): "a" (syscall): "memory","cc");
   return ret;
}


