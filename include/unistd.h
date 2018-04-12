#ifndef _UNISTD_H
#define _UNISTD_H

extern long sb_chdir(char * str);

extern void sb_pwd(int print);

extern void sb_pipe(int fd);

extern void sb_dup2(int ofd,int nfd);

extern long sb_fork();

extern void sb_close(long fd);

extern void sb_exit(int code);

extern int sb_execve(char *fd, char **argv, char **env);

extern void * sb_malloc(int size);
extern int  sb_yield();
//int open(const char *pathname, int flags);
//int close(int fd);
//ssize_t read(int fd, void *buf, size_t count);
//ssize_t write(int fd, const void *buf, size_t count);
//int unlink(const char *pathname);

//int chdir(const char *path);
//char *getcwd(char *buf, size_t size);

//pid_t fork();
//int execvpe(const char *file, char *const argv[], char *const envp[]);
//pid_t wait(int *status)
//int waitpid(int pid, int *status);

//unsigned int sleep(unsigned int seconds);

//pid_t getpid(void);
//pid_t getppid(void);

// OPTIONAL: implement for ``on-disk r/w file system (+10 pts)''
//off_t lseek(int fd, off_t offset, int whence);
//int mkdir(const char *pathname, mode_t mode);

// OPTIONAL: implement for ``signals and pipes (+10 pts)''
//int pipe(int pipefd[2]);

#endif
