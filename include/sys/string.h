#ifndef _STRING_H
#define _STRING_H


int strlen(char *a);
void strcpy(char* src,char* dest);
int strcmp(char *a, char *b,int len);
void memcpy(void *dest, void *src, uint64_t len);
int strstr(char *str,char *sub);
void memset(void * ptr,int val,int size);
char* strtok(int toknum,char* argv1, char separator);

#endif


