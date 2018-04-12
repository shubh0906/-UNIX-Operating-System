
#include "syscall.h"
#include "string.h"
#include "sysdef.h"
#include "stdio.h"
#include "printf.h"

int main(int argc,char* argv[],char* envp[])
{
//	char inp[1000];
	int i=1;
	printf("-------------%d\n",argc);
	if (argc>1){
		for(;i<argc;i++)
		printf("%s ",argv[i] );
	}
	printf("\n");
	return 0;	
}

