#include "../../include/stdio.h"
#include "../../include/printf.h"
#include "../../include/string.h"
#include "../../include/unistd.h"
#include "../libc/ls.c"
#include "../libc/echo.c"
#include "../libc/cat.c"
#include "../libc/sleep.c"
#include "../../include/wait.h"


/*char * cd(char * buf, char* path)
{
    char * tempbuf=sb_malloc(10*sizeof(buf));
    _strcpy_(buf,tempbuf);
    char * newpath;//=sb_malloc(10*sizeof(buf));
    //_strcpy_(tempbuf,newpath);
    newpath=tempbuf;
    int buflen=_strlen_(buf);

    int j=1;
    char*token=split(j,path,'/');
    //path[_strlen_(path)-1]='\0';
    //put_s(token);
    while (token) {
        //put_s("token");
        //put_s(token);
        //put_s("token1");
        put_s("\n");
        if (!_strcmp(token, "..",0)) {
            //put_s("in if");
            int i = 1;
            newpath = tempbuf;

            while (tempbuf[buflen - i] != '/') {
                newpath[buflen - i] = '\0';
                i++;
            }
            newpath[buflen - i] = '\0';

            if (_strlen_(newpath) == 0) {
                newpath[0] = '/';
            }
        } else {
            //puts("in else");
            if (token[0] == '/') {
                newpath = token;
                newpath[_strlen_(token)] = '\0';
                break;
            }
            else{
                if (!_strcmp(token, ".",0)){
                    j++;
                    token=split(j,path,'/');
                }
                int i = 0;
                newpath[buflen] = '/';
                //newpath[buflen + 1] = '\0';
                for (i = buflen + 1; i < _strlen_(token) + buflen + 1; i++) {
                    newpath[i] = token[i - buflen - 1];
                }
                newpath[i] = '\0';
            }
        }
        //put_s("out");
        //put_s(newpath);
        _strcpy_(newpath,tempbuf);
        buflen=_strlen_(tempbuf);
        //put_s(tempbuf);
        put_s("\n");

        if (path[_strlen_(path)-1]=='/' && path[_strlen_(path)-2]=='/'){
            //return tempbuf;
            break;
        }

        j++;
        token=split(j,path,'/');
        //path[_strlen_(path)-1]='\0';
        //put_s(token);
    }
    return tempbuf;
}*/


/*char* split(int toknum, char* argv1, char separator){

    char **argv2=(char**)sb_malloc(10* sizeof(char*));
    for (int i=0;i<10;i++){
        argv2[i]=(char*)sb_malloc(10* sizeof(char));
    }
    int ss=0,s=0,d=0,d1=0;
    //int flag=0;

    //static char argv2[][];
    if ( argv1[_strlen_(argv1)-1]!=separator ){
        argv1[_strlen_(argv1)]=separator;
        argv1[_strlen_(argv1)+1]='\0';
    }
    if (argv1[0]==separator){
        argv1=argv1+1;
    }
//argv2[ss]=(char *)malloc(100);

    for (s=0;s<_strlen_(argv1);s++)
    {
        if(argv1[s]==separator)
        {
            argv2[ss][d]='\0';

            puts(argv2[ss]);
            ss++;
            d=0;
            d1++;
            //flag=0;
            //argv2[ss]=(char*)sb_malloc(10);
        }
        else
        {
            if (argv1[s]=='\0' || argv1[s]=='0'){
                continue;
            }
            else {
                //printf("%c",argv1[d1]);
                argv2[ss][d] = argv1[d1];
                d++;
                d1++;
                //flag=1;
            }
        }
    }
    //argv2[ss][d]='\0';
    //if (flag==1){
        //argv2[ss+1][0]='\0';
    //}
//return argv2;

    for (int i=ss-1;i<10;i++){
        argv2[i][0]='\0';
    }

    char* ans=argv2[toknum-1];
    if (_strcmp(ans,"",1)){
        char * ans=argv2[toknum-1];
//puts(argv2[toknum]);
        return ans;
    }
    else{
//printf("no token");
        return NULL;
    }
//argv2[100][100]={" "};
}*/


int main(int argc,char * argv[],char *envp[])
{

    //volatile int a=0;
    //int x=10;
    //char * aa="shrinand\n";
    //char * bb;
    //printf("%d zxcv",x);
    //put_s(aa);
    //put_s("\n");
    //printf("%d qwerty",x);
    //_gets_(aa);

    //put_s(aa);

    //sb_pwd();
    //char * path="bin/2/1/b.txt";
    //cat(path);
    //char * path1="bin/2/1/a.txt";
    //cat(path1);
    //_gets_(aa);
    //ls();
    //echo("shrinand qwerty");
    //char * aa =(char*) sb_malloc(5000);
    //_gets_(aa);
    //put_s(aa);
    //clearScreen();
    //put_s("zxcv");

    /*char * aa="ab/bqw";
    put_s(split(1,aa,'/'));
    put_s("\n");
    put_s(split(2,aa,'/'));
    put_s("\n");
    put_s(split(3,aa,'/'));
    put_s("\n");
    put_s(split(4,aa,'/'));
    put_s("\n");
    put_s(split(5,aa,'/'));*/

    /*char * aa="rootfs/bin/hello";

    char* firstToken=split(1,aa,'/');

    //put_s(aa);
    //firstToken="";
    if (!_strcmp(firstToken,"rootfs",0)){
        if (*(aa)=='/'){
            *aa=*aa+8;
        }
        else{
            *aa=*aa+7;
        }
    }*/
    char *aa=sb_malloc(100*(sizeof(char)));
    //*aa=sb_malloc(100* sizeof(char));
    _gets_(aa);
    int i=1;
    char ** ans=sb_malloc(100*sizeof(char*));
    char * token = sb_malloc(20*sizeof(char));
    ans[0]=sb_malloc(20*sizeof(char));
    token=split(1,aa,' ');
    _strcpy_(token,ans[0]);
    while (1)
    {
            ans[i]=sb_malloc(20*sizeof(char));
            token=split(i+1,aa,' ');
            if(_strcmp(token,"",0)==0)
                break;
            _strcpy_(token,ans[i]);
            //printf("%s\n",ans[i] );
            i++;
    }
    /*if (aa[_strlen_(aa)-1]=='/'){
        aa[_strlen_(aa)-1]='\0';
    }
    put_s("\n");
    char*aa1=sb_malloc(100* sizeof(char));
    _gets_(aa1);

    char * path = cd(aa,aa1);



    //put_s("hey");
    put_s(path);
    */
    //ls();

    //sleep(5);

    /*a=sb_fork();
    // /printf("\n%d\n",a );
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
    }*/
        while(1);
}
