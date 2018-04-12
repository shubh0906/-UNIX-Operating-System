#ifndef _STDIO_H
#define _STDIO_H

static const int EOF = -1;

int putchar(int c);
int puts(const char *s);
//int printf(const char *format, ...);

extern char get_ch();

extern void _gets_(char *str);

extern void put_ch(char str);

extern void put_s(char *str);

extern char *gets(char *s);

extern long sb_fopen(char *path);

extern long sb_fread(long fd);
#endif
