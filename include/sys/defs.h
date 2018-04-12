#ifndef _DEFS_H
#define _DEFS_H

#define NULL ((void*)0)

typedef unsigned long  uint64_t;
typedef          long   int64_t;
typedef unsigned int   uint32_t;
typedef          int    int32_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;
typedef unsigned char   uint8_t;
typedef          char    int8_t;

typedef uint64_t size_t;
typedef int64_t ssize_t;

typedef uint64_t off_t;

typedef uint32_t pid_t;


typedef struct registers
{
   	uint64_t r9,r8;

	uint64_t rsi,rbp,rdx,rcx,rbx,rax,rdi;
	
	uint64_t int_no,err_code;
	uint64_t rip,cs,rflags,rsp,ss;
} registers_t;

#endif
