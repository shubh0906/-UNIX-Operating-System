#ifndef _DIRENT_H
#define _DIRENT_H

#include <sys/defs.h>
#define NAME_MAX 255

#define FILE 1
#define DIRECTORY 2
struct linux_dirent  {
               unsigned long  fd_ino;
               unsigned long  fd_off;  
               unsigned short fd_reclen;
               unsigned char  fd_type; 
               char           fd_name[
];             };

extern long readdir(long read);


extern int opendir(char *dir);

struct dirent {
 char d_name[NAME_MAX+1];
};

typedef struct DIR DIR;


typedef struct {
        char name[100];
        int size;
        int typeflag;
        uint64_t start;
        int parent;
}fnode;
//DIR *opendir(const char *name);
//struct dirent *readdir(DIR *dirp);
//sint closedir(DIR *dirp);

#endif
