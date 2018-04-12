#include "syscall.h"
#include "string.h"
#include "dirent.h"
#include "unistd.h"
#include "sysdef.h"
#include "stdio.h"
#include "printf.h"


void  ls()
{
	int n=0;
	//struct linux_dirent *d;
	n=opendir(temp1);
	printf("1------------------ %d\n",n );
	if(n!=-1)
    n=readdir(n);
    /*int i=0;
    for (i = 0;i < n;) 
    {
        d = (struct linux_dirent *) (direc + i);
        put_s(d->fd_name);
        put_s("\t");
        i += d->fd_reclen;
    }*/
    put_s("\n");
}

int main(int argc,char* argv[],char* envp[])
{
	//char inp[1000];
	//put_s("sbush.ls>");
	//_gets_(inp);
	//put_s(inp);
	/*if(argc>1)
		_strcpy_(argv[1],pwd);
	else*/
	//put_s("1--------------------");
	char*pwd=sb_malloc(20* sizeof(char));
	if(argc>1)
		_strcpy_(argv[1],pwd);
	//else
		//sb_pwd(0);
	//printf("1  %s\n",pwd );
	char* firstToken=split(1,pwd,'/');
	//printf("firstToken %s\n",firstToken );

	if (strncmp(firstToken,"rootfs",6)){
		//printf("1\n");
		if (*(pwd)=='/'){
			int i=0;
			while(pwd[i+8]!='\0')
			{
				temp1[i]=pwd[i+8];
				i++;
			}
			//pwd=pwd+8;
			//printf("2\n");
		}
		else{
			int i=0;
			while(pwd[i+7]!='\0')
			{
				temp1[i]=pwd[i+7];
				i++;
			}
			
			//printf("3\n");
			//pwd=(pwd+7);
		}
		}

	temp1[_strlen_(temp1)-1]='\0';

	if (_strlen_(temp1) == 0){
		temp1[0]='/';
		temp1[1]='\0';
	}
	//printf("2   %s\n",temp1 );
	ls();
	return 0;	
}
