#include "unistd.h"
#include <stdlib.h>
#include "stdio.h"
#include "sysdef.h"
#include "sys/defs.h"
void _start(void) {
  // call main() and exit() here
	int *rsp=0;
	unsigned long *argc;
	char **argv=NULL;
	__asm __volatile("movq %%rsp, %0;":"=a" (rsp)::"memory","cc");
	
	argc=((unsigned long*)rsp+2);
	argv=((char**)(*((char **)rsp+0x1)));
	if(*argc>1)
		envp=argv+(*argc-1)+0x2;
	else
		envp=argv+0x2;
	int res;
	res=main(*argc,argv,envp);
	//while(1);
	//put_s("in start");
	sb_exit(res);
}


