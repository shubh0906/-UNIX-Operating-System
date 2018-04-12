#include "../include/string.h"
#include "../include/stdio.h"
#include "../include/printf.h"
#include "../include/unistd.h"

char * cd(char * buf, char* path)
{
    if (buf[_strlen_(buf)-1]=='/'){
        buf[_strlen_(buf)-1]='\0';
    }
    if (buf[0]!='/'){
        char * temp=sb_malloc(20* sizeof(char));
        char*temp1=temp+1;
        _strcpy_(buf,temp1);
        temp[0]='/';
        _strcpy_(temp,buf);
    }
    if (!_strcmp(buf,"//",0)){
        buf[1]='\0';
    }
    char * tempbuf=sb_malloc(20*sizeof(buf));
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
        //put_s("\n");
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
            if (path[0] == '/') {
                newpath = token;
                newpath[_strlen_(token)] = '\0';
                int length=_strlen_(path);
                int qq=1;
                while (qq<length) {
                    tempbuf[qq - 1] = path[qq];
                    qq++;
                }
                tempbuf[qq-1]='\0';
                if (_strlen_(tempbuf)==0){
                    tempbuf[0]='/';
                    tempbuf[1]='\0';
                }
                //while (qq<)
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
        //put_s("\n");

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
}