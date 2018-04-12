//#include<stdlib.h>
//#include<stdio.h>
//#include<unistd.h>
//#include<string.h>

#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "syscall.h"
#include "sysdef.h"
#include "wait.h"
#include "dirent.h"
#include "../libc/ls.c"
#include "../libc/cd.c"
#include "../libc/syscall.c"
#include "../libc/cat.c"
#include "../libc/shell.c"
#include "../libc/malloc.c"
#include "../include/sys/defs.h"



/*char * cd(char * buf, char* path)
{

    char * newpath=buf;
    int buflen=_strlen_(buf);

    if(!_strcmp(path,"..",0))
    {   
        int i=1;
        newpath=buf;

        while (buf[buflen-i]!='/')
        {
            newpath[buflen-i]='\0';
            i++;
        }
        newpath[buflen-i]='\0';

        if(_strlen_(newpath)==0)
        {
            newpath[0]='/';
        }
    }
    else
    {
        if(path[0]=='/'){
            newpath=path;
            newpath[_strlen_(path)]='\0';
        }
        else
        {
            int i=0;
            newpath[buflen]='/';
            newpath[buflen+1]='\0';
            for (int i=buflen+1;i<_strlen_(path)+buflen+1;i++)
            {
                newpath[i]=path[i-buflen-1];
            }
            newpath[i]='\0';
        }
    }

    sb_chdir(newpath);
    //printf(" %d\n",result);
    return newpath;
}*/


int main(int argc, char*argv[],char *envp[]){

    sb_pwd(0);
    int flag1=0;

    volatile int a=0;
//_clearscreen_(0x4E);
    while (1) {
        printf("sbush: %s $",pwd);
        char* input=(char*)malloc(30* sizeof(char));
        _gets_(input);

        char * binary = (char*)malloc(30* sizeof(char));
        start:
        binary = split(1,input,' ');


        if (!_strcmp(binary,"ls",0)) {

            //put_s(binary);
            //put_s(" called");
            //put_s("\n");

            int i=1;
            char ** ans=(char**)malloc(100*sizeof(char*));
            ans[0]=(char*)malloc(30*sizeof(char));
            _strcpy_(binary,ans[0]);

            char* nextToken=(char*)malloc(30*sizeof(char));
            while (1) {
                ans[i]=(char*)malloc(30*sizeof(char));
                nextToken=split(i+1,input,' ');
                if(_strcmp(nextToken,"",0)==0)
                    break;
                _strcpy_(nextToken,ans[i]);
                //printf("%s\n",ans[i] );
                i++;
            }

            if (!_strcmp(ans[1],"",0)){
                _strcpy_(pwd,ans[1]);
                ans[2]="";
            }

            a=sb_fork();
            // /printf("\n%d\n",a );
            //put_s("here");
            if(a==0) {
                //printf("in child\n" );
                sb_execve("bin/ls",ans,NULL);
                //sb_yield();
            }
            else {
                //printf("in parent");
                sb_waitpid(a,0,0);
                //printf("back to parent");
            }
            put_s("done");
            free((uint64_t)ans);
            free((uint64_t)nextToken);
        }

        else if (!_strcmp(input,"pwd ",0)) {
            printf(" : ");
            //sb_pwd(1);
            put_s(pwd);
        }

        else if (!_strcmp(input,"ps ",0)){
            char ** ans=(char**)malloc(100*sizeof(char*));
            ans[0]=(char*)malloc(30*sizeof(char));
            _strcpy_("ps",ans[0]);

            ans[1]=(char*)malloc(30*sizeof(char));
            ans[1]="";

            a=sb_fork();
            // /printf("\n%d\n",a );
            //put_s("here");
            if(a==0) {
                //printf("in child\n" );
                sb_execve("bin/ps",ans,NULL);
                //sb_yield();
            }
            else {
                //printf("in parent");
                sb_waitpid(a,0,0);
                //printf("back to parent");
            }
            free((uint64_t)ans);
        }

        else if (!_strcmp(binary,"cat",0)){
            //put_s(binary);
            //put_s(" called");
            //put_s("\n");
            //put_s("in cat");
            /*int i=2;
            char* nextToken=split(i,input,' ');
            //put_s("\n");
            //put_s(nextToken);
            //put_s("\n");
            while(_strcmp(nextToken,"",0)){
                nextToken=sb_malloc(20* sizeof(char));
                i++;
                nextToken=split(i,input,' ');
                put_s(nextToken);
                put_s("\n");
            }*/
            int i=1;
            char ** ans=(char**)malloc(100*sizeof(char*));
            ans[0]=(char*)malloc(30*sizeof(char));
            _strcpy_(binary,ans[0]);

            char* nextToken=(char*)malloc(30*sizeof(char));
            while (1)
            {
                ans[i]=(char*)malloc(30*sizeof(char));
                nextToken=split(i+1,input,' ');
                if(_strcmp(nextToken,"",0)==0)
                    break;
                _strcpy_(nextToken,ans[i]);
                //printf("%s\n",ans[i] );
                i++;
            }

            a=sb_fork();
            // /printf("\n%d\n",a );
            //put_s("here");
            if(a==0)
            {
                //printf("in child\n" );
                sb_execve("bin/cat",ans,NULL);
                //sb_yield();
            }
            else
            {
                //printf("in parent");
                sb_waitpid(a,0,0);
                //printf("back to parent");
            }
            free((uint64_t)ans);
            free((uint64_t)nextToken);
        }


        else if (!_strcmp(binary,"sleep",0)){
            //put_s(binary);
            //put_s(" called");
            //put_s("\n");
            //put_s("in echo");
            /*int i=2;
            char* nextToken=split(i,input,' ');
            //put_s("\n");
            //put_s(nextToken);
            //put_s("\n");
            while(_strcmp(nextToken,"",0)){
                nextToken=sb_malloc(20* sizeof(char));
                i++;
                nextToken=split(i,input,' ');
                put_s(nextToken);
                put_s("\n");
            }*/

            char ** ans=(char**)sb_malloc(100*sizeof(char*));
            ans[0]=(char*)malloc(30*sizeof(char));
            _strcpy_(binary,ans[0]);

            char* nextToken=(char*)malloc(30*sizeof(char));
            //while (1)
            //{
            ans[1]=(char*)malloc(30*sizeof(char));
            nextToken=split(2,input,' ');
            _strcpy_(nextToken,ans[1]);
            ans[2]=(char*)malloc(30*sizeof(char));
            _strcpy_("",ans[2]);

            a=sb_fork();
            // /printf("\n%d\n",a );
            //put_s("here");
            if(a==0)
            {
                //printf("in child\n" );
                sb_execve("bin/sleep",ans,NULL);
                //sb_yield();
            }
            else
            {
                //printf("in parent");
                sb_waitpid(a,0,0);
                //printf("back to parent");
            }
            free((uint64_t)ans);
            free((uint64_t)nextToken);
        }


        else if (!_strcmp(binary,"echo",0)){
            //put_s(binary);
            //put_s(" called");
            //put_s("\n");
            //put_s("in echo");
            /*int i=2;
            char* nextToken=split(i,input,' ');
            //put_s("\n");
            //put_s(nextToken);
            //put_s("\n");
            while(_strcmp(nextToken,"",0)){
                nextToken=sb_malloc(20* sizeof(char));
                i++;
                nextToken=split(i,input,' ');
                put_s(nextToken);
                put_s("\n");
            }*/

            int i=1;
            char ** ans=(char**)malloc(50*sizeof(char*));
            ans[0]=(char*)malloc(30*sizeof(char));
            _strcpy_(binary,ans[0]);

            char* nextToken=(char*)malloc(30*sizeof(char));
            while (1)
            {
                ans[i]=(char*)malloc(30*sizeof(char));
                nextToken=split(i+1,input,' ');
                if (!nextToken[0]){
                    //put_s("NULL");
                    _strcpy_("",ans[i]);
                    break;
                    //nextToken="";
                }
                //put_s("\n\n");
                printf("--%s--",nextToken);
                put_s("\n\n");
                if(_strcmp(nextToken,"",0)==0) {
                    _strcpy_(nextToken,ans[i]);
                    break;
                }
                _strcpy_(nextToken,ans[i]);
                    //printf("-----------%s-----------\n",ans[i] );
                //put_s("start\n");
                //put_s(ans[i]);
                //put_s("\nend");
                i++;
            }

            /*put_s("\nQWERTY Going for Fork\n");

            printf("%x -- %s\n",&ans[0],ans[0]);
            printf("%x -- %s\n",&ans[1],ans[1]);
            printf("%x -- %s\n",&ans[2],ans[2]);
            _strcpy_("",ans[2]);*/

            a=sb_fork();
            // /printf("\n%d\n",a );
            put_s("\nhere in fork\n");
            if(a==0)
            {
                //printf("in child\n" );
                sb_execve("bin/echo",ans,NULL);
                //sb_yield();
            }
            else
            {
                //printf("in parent");
                sb_waitpid(a,0,0);
                //printf("back to parent");
            }
            free((uint64_t)ans);
            free((uint64_t)nextToken);

        }

        else if (!_strcmp(binary,"cd",0)){
            //put_s(binary);
            //put_s(firstToken);
            //put_s("\n");
            //put_s("in cd");
            int i=2;
            char* nextToken=split(i,input,' ');
            //put_s("\n");
            //put_s(nextToken);
            //put_s("\n");
            char * newpwd=sb_malloc(30* sizeof(char));
            char * oldpwd=sb_malloc(30* sizeof(char));
            _strcpy_(pwd,oldpwd);
            newpwd = cd(pwd,nextToken);
            char * storedpwd = sb_malloc(30* sizeof(char));
            if (newpwd[0]=='/' && newpwd[1]=='/'){
                //put_s("yo");
                char * temp=sb_malloc(30* sizeof(char));
                char * temp1=temp+1;
                _strcpy_(newpwd,temp);
                //put_s("\n");
                //put_s(temp1);
                _strcpy_(temp1,newpwd);
            }
            if (newpwd[0]!='/'){
                char * temp=sb_malloc(30* sizeof(char));
                char * temp1=temp+1;
                temp[0]='/';
                _strcpy_(newpwd,temp1);
                _strcpy_(temp,newpwd);
            }

            _strcpy_(newpwd,storedpwd);

            char * dupnewpwd=sb_malloc(30* sizeof(char));
            _strcpy_(newpwd,dupnewpwd);
            char* firstToken=split(1,dupnewpwd,'/');
            //put_s(firstToken);
            //_strcpy_(newpwd,newpwd);

            char * checkifDir=sb_malloc(30* sizeof(char));
            _strcpy_(newpwd,checkifDir);
            if (!_strcmp(firstToken,"rootfs",6)){
                //*checkifDir=*checkifDir+7;
                //put_s("\n");
                //put_s(&checkifDir[7]);
                _strcpy_(&checkifDir[7],newpwd);
                //put_s(newpwd);
                //put_s("\n");
            }



            if (newpwd[0]=='/' && _strlen_(newpwd)==1){
                _strcpy_("/rootfs",pwd);
                _strcpy_(newpwd, &pwd[7]);
            }

            else if (newpwd[0]=='/'){
                //put_s("here");
                //put_s(&newpwd[1]);
                if (_chdir_(0x0C,&newpwd[1])) {
                    //sb_pwd(1);
                    //printf("set here %s",&newpwd[1]);
                    _strcpy_("/rootfs",pwd);
                    _strcpy_(newpwd, &pwd[7]);
                }
                else{
                    _strcpy_(oldpwd, pwd);
                }
            }
            else{
                //put_s(&newpwd[0]);
                if (_chdir_(0x0C,&newpwd[0])) {
                    _strcpy_("/rootfs",pwd);
                    _strcpy_(newpwd, &pwd[7]);
                }
                else{
                    _strcpy_(oldpwd, pwd);
                }
            }
        }
        else if (_strstr(input,".sh")){
            char ** ans=sb_malloc(100*sizeof(char*));
            ans[0]=sb_malloc(30*sizeof(char));
            _strcpy_(binary,ans[0]);

            char* nextToken=sb_malloc(30*sizeof(char));
            ans[1]=sb_malloc(30*sizeof(char));
            nextToken=split(2,input,' ');
            _strcpy_(nextToken,ans[1]);
            //sh(ans[1]);

            //int n=0;
            _strcpy_(ans[1],temp);
            put_s(temp);
            //mode=0;
            //printf("%d",mode);
            //n=_open_(0x05,temp,0);
            //printf("%d abcd",n);
            //n=_read_(3 ,(long)n,buf,1000);
            char * retfile = sb_malloc(30* sizeof(char));
            retfile=shell(ans[1]);

            char * line = sb_malloc(30* sizeof(char));
            _strcpy_(retfile,line);
            line = split(1,buf,'\n');
            //put_s("here before shebangs");
            put_s(line);
            if (!_strcmp(line,"#!sbush",7)) {
                int i1 = 1;
                char **ans = sb_malloc(100 * sizeof(char *));
                ans[0] = sb_malloc(30 * sizeof(char));
                _strcpy_(line, ans[0]);
                //put_s("QWERTY");
                put_s(line);
                put_s("\n");
                char *nextToken = sb_malloc(30 * sizeof(char));
                while (1) {
                    flag1 = 1;
                    ans[i1] = sb_malloc(30 * sizeof(char));
                    nextToken = split(i1 + 1, buf, '\n');
                    if (_strcmp(nextToken, "", 0) == 0) {
                        flag1 = 0;
                        break;
                    }
                    _strcpy_(nextToken, ans[i1]);
                    //printf("CHECK%s\n", ans[i1]);
                    int ll=_strlen_(ans[i1]);
                    while (nextToken[ll-1]==' '){
                        ll--;
                    }
                    input = sb_malloc(30 * sizeof(char));
                    //_strcpy_(ans[i1],input);
                    input  = ans[i1];

                    goto start;
                    shebang:
                    i1++;
                }
            }



        }
        put_s("\n");
        //printf("sbush: %s ",pwd);
        //put_s(" @");
        if (flag1==1){
            goto shebang;
        }
    }
}

/*else if(_strstr(argv,"|")){

int fd[2];
int i=1;
int status=0;
//char argv1[]="hello";//=(char*)malloc(100);
//char **argv2=
split(argv,'|');

int count=0;

for(int j=0;j<_strlen_(argv);j++){
if(argv[j]=='|')
count++;
}

split_string(argv2[0],' ');
long pid;
sb_pipe(fd);

        while(i<count+1){

        //puts("hello");
        sb_pipe(fd);
        //a=
        //puts("hello");
        //split1(argv2[i-1],' ');
        split_string(argv2[i-1],' ');
        //puts("hell");
        //puts(a[0]);
        pid=sb_fork();
        if(pid==0){
        sb_close(1);
        sb_dup2(fd[1],1);
        //puts(a[0]);
	sb_execve(a[0],a,envp);
        }
        else{
        sb_waitpid(pid,&status,0);
        sb_dup2(fd[0],0);
        sb_close(fd[1]);
        i++;
        }
        }

//puts("bye");
split_string(argv2[i-1],' ');
//split1(argv2[i-1],' ');
pid=sb_fork();
if(pid==0){
sb_execve(a[0],a,envp);
}
else
sb_waitpid(pid,&status,0);

}*/

/*else{
        long pid;
        int waitChild=0;
        int status=0;
	char *bin="/bin/";
        //while(1)
        {
		//put_s(bin);
		int bb=0;
		int len=_strlen_(argv);
		while(bb<=len){
		//put_ch(argv[bb]);
		//puts("\n");
		bin[bb+5]=argv[bb];
		//put_s(bin);
		bb++;
		}
		//put_s(bin);
		bin[bb+7]='\0';
		_strcpy_(bin,argv);
                put_s(argv);
		if(argv[_strlen_(argv)-1] == '&')
                {
                        waitChild= 1;
                        argv[_strlen_(argv)-2]='\0';
                }

                pid = sb_fork();
                if (pid == 0)
                {
                        //puts("child");
                        if(waitChild==1)
                        {
                                //puts("hello");
                                waitChild=0;
                                //sleep(1);
                        }
                        //split_string(argv,' ');
                        //argv2[2][0]='\0';
                        //sb_execve(a[0],a,envp);
			//exit(1);
                }
                else
                {
                        if (waitChild==1){
                                //waitChild=0;
                                sb_waitpid(pid,&status,0);
                                waitChild=0;
                        }
                        //puts("parent");
                }

                //char *argv1=(char *)malloc(100);
                //if(gets(argv1));
                //free(argv);

}

}
}
return 0;
}*/


//put_s(argv2[0]);
/*
if(_strcmp("qwerty","qwerty",0)){
put_s("not");
}*/

//if(!_strcmp(argv2[0],"cd",0))
//if(argv2[0][0]=='c' && argv2[0][1]=='d')
//{
//put_s("hello");
//char buf[100];//=(char *)malloc(100);
//char * pwd= 
//sb_pwd();
//put_s(argv2[1]);
/*char * newpath=cd(pwd,argv2[1]);
put_s(newpath);
}*/
/*
else if(!_strcmp(argv2[0],"ls",0) || !_strcmp(argv2[0],"echo",0) || !_strcmp(argv2[0],"pwd",0)){

	pid_t pid;
	int waitChild=0;
	int* status;
	//while(1)
	{	

		if(argv[_strlen_(argv)-1] == '&')
                {
                        waitChild= 1;
                        put_s("waitChild");
                        put_s(argv);
                        argv[_strlen_(argv)-2]='\0';
                        put_s(argv);
                }
	
		pid = sb_fork();
		if (pid == 0)
		{
			put_s("child");
			if(waitChild==1)
			{
				put_s("hello");
				waitChild=0;
				//sleep(1);
			}
			//puts(argv);
			//puts("check");
			//argv2=
			split(argv,' ');
			//char *yy[]={"ls",NULL};
			//int y=exebin(argv2);
			//char **hgh=(char**)malloc(1000);
			//char te[100][100]={{}};
			//te[0]=&argv[0];
			char str[100];
			str[0] = '\0';
			
			for(int i=0;argv2[2][i]!='\0';i++,_strcpy_(&argv2[2][i],NULL));
			
			//argv2[2][0]='\0';
			int y=execvp(argv2[0],argv2);
			puts("ghjsd");
			puts(argv2[0]);
			puts(argv2[1]);
			puts(argv2[2]);
			//printf("value returned is %d",y);
			exit(1);
		}
		else
		{
			if (waitChild==1){
				//waitChild=0;
				pid=wait(status);
				waitChild=0;
			}               
			puts("parent");
		}	
		
		//char *argv1=(char *)malloc(100);
                //if(gets(argv1));
		
		//puts(argv2[0]);
                //puts(argv2[1]);
                //puts(argv2[2]);



		//free(argv);
	}

}
*/

/*
else if (!_strcmp(argv2[0],"export",0)){

char *path;

if(argv2[1][1]=='A')
path="PATH";
else
path="PS1";

int ss=0,s=0,d=0,d1=0,flag=0;
char *s1;//=(char*)malloc(100);
s1=getenv(path);
//put_s(argv2[1]);
//char *argv1=(char *)malloc(100);
//if(gets(argv1));

//char **argv2=(char **)malloc(100);
//argv2[ss]=(char *)malloc(100);

                for (s=0;s<_strlen_(argv);s++)
                {
                        if(argv[s]=='$'){flag=1;}
                        if(argv[s]==':')
                        {
                                argv2[ss][d]='\0';
                                ss++;
                                d=0;
                                d1++;
                                //argv2[ss]=(char *)malloc(100);
                        put_s(argv2[1]);
                        put_s("jj");
                        }
                        else
                        {
                                argv2[ss][d]=argv[d1];
                                d++;d1++;
                        }
                }
                argv2[ss][d]='\0';
        if(flag==1){
                //puts("pp");
                //puts(s1);
                //puts(argv2[1]);
                int uu=_strlen_(s1);
                //printf("\n value of s1 is %d\n",strlen(s1));
                s1[uu]=':';
                //printf("value of s1 %d",strlen(s1));
                for(s=0;s<_strlen_(argv2[1]);s++){
                        s1[uu+1+s]=argv2[1][s];
                }
                s1[uu+s+1]='\0';
        }
        else{
                //char ** fff=
                split(argv,' ');
                s1=argv2[1];
        }
put_s(s1);
*/


        
//}                      
                         
//return 0;
//}

