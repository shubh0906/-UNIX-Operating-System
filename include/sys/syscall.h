#ifndef SYSCALL_H
#define SYSCALL_H
uint64_t syscall(uint64_t sysc,uint64_t arg1,uint64_t arg2,uint64_t arg3);
uint64_t waitpidSyscall(uint64_t pid, int status, int options);
uint64_t exitSyscall(int code);
uint64_t sleepSyscall(int64_t seconds);
uint64_t dup2Syscall(int64_t ofd,int64_t nfd);
uint64_t writeSyscall(int64_t fd, int64_t str,int64_t count);
uint64_t readSyscall(int64_t fd, uint64_t address, int64_t length);
uint64_t getPIDSyscall();
uint64_t getCurrentDirectorySyscall();
int openSyscall(char * path,uint64_t mode);
uint64_t chdirSyscall(char * path);
uint64_t getpwdSyscall(uint64_t buffer);

int readdirSyscall(uint64_t addr);

uint64_t mallocSyscall(uint64_t size);

int forkSyscall();
uint64_t execveSyscall(char * filename, uint64_t str, uint64_t count);
uint64_t yieldSyscall();
uint64_t clearscreenSyscall();
void listprocess();
uint64_t exitSyscall(int code);
//int sys_opendir(char * path);

#endif
