#ifndef _STRING_H
#define _STRING_H

extern int _strlen_(char *a);

extern int _strcmp(char *a, char *b,int len);

extern void _strcpy_(char* src,char* dest);

extern int _strstr(char *str, char *sub);

extern char* split(int toknum,char* argv1, char separator);
int strncmp(char *a, char *b,int len);
void memset(void * ptr,int size);

#endif
