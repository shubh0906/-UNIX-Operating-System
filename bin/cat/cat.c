#include "syscall.h"
#include "string.h"
#include "sysdef.h"
#include "stdio.h"
#include "printf.h"

void cat(char *inp) {
   	int n=0;
        _strcpy_(inp,temp);
	n=_open_(0x05,temp,0);
	if (n!=-1){
		n=_read_(3  ,(long)n,buf,1000);
		put_s(buf);
	}
}

int main(int argc,char* argv[],char* envp[])
{
//	char inp[1000];


	//put_s("in cat");
	//printf("\n%d\n",argc);
	int i=1;
	if (argc==i){
		put_s("No Files Passed");
	}
	while(i<argc)
	{
		//put_s(argv[i]);
		//put_s("\n");
		//put_s("sbush.cat>");
		//_gets_(inp);
		//:put_s(inp);
		cat(argv[i]);
		i++;
	}
	return 0;	
}

