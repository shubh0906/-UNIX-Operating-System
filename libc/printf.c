#include <stdarg.h>
#include "../include/printf.h"
#include "../include/stdio.h"
#include "../include/syscall.h"



va_list args;
char x[1000];
char d[1000];
char p[1000];
int xp=0,yp=0;
int color=7;
//volatile char* video=(volatile char *)0xB8000;
void prints(char *fmt);
void printd(int num);
void convert(unsigned long dec,int base, char type);
char buffer[512];
int index=0;

void clearScreen(){
    _clearscreen_(0xAA);
}

/*void memset()
{
    xp=0;yp=0;
    uint64_t tempAddress;
    uint64_t vidTempAddress ;
    char* tempV=(char *)0xB8000;
    for(int i=0;i<3200;i++)
    {
        if(xp==80)
        {
            yp++;
            xp=0;
        }
        tempAddress = (uint64_t)0xB8000+((yp+1)*160+xp);
        vidTempAddress  =(uint64_t)0xB8000+((yp)*160+xp);
        tempV=(char *)tempAddress;
        video=(char *)vidTempAddress;
        *video=*tempV;
        //video=color;
        xp++;
    }
}*/
/*void scroll()
{
    uint64_t tempV=(uint64_t)((char*)0xffffffff800B8000);
    yp=22;
    memcpy((void*)tempV,(void*)tempV+160,23*160);
    memset((void*)(tempV+23*160),0,160);
    video=((char*)0xffffffff800B8000)+yp*160;

}
void write(char c)
{
    *video++=c;
    *video++=color;
    if(xp<80)
        xp++;
    else
    {
        xp=0;
        if(yp<22)
        {
            yp++;
            //  printd(yp);
        }
        else
        {
            scroll();
            //      memset();
        }
    }
}*/
void convert(unsigned long dec,int base, char type)
{
    char fmt[20];
    int i=0,rem=0,j=0;
    do
    {
        rem=dec%base;
        if(rem==15)
            fmt[i]='f';
        else if (rem==14)
            fmt[i]='e';
        else if (rem==13)
            fmt[i]='d';
        else if (rem==12)
            fmt[i]='c';
        else if (rem==11)
            fmt[i]='b';
        else if (rem==10)
            fmt[i]='a';
        else
            fmt[i]=(char)(rem+48);
        dec=dec/base;
        i++;
    }while(dec>0);
    if(type=='p')
    {
        fmt[i]='x';
        i++;
        fmt[i]='0';
        i++;
    }
    while(i>0)
    {
        i--;

        if(type=='c')
        {
            //write(fmt[i]);
            buffer[index]=fmt[i];
            index++;
        }
        else if(type=='d')
        {
            d[j]=fmt[i];
        }
        else
        {
            x[j]=fmt[i];
        }
        j++;
    }
    if(type=='x')
        x[j]='\0';
    if (type=='d')
        d[j]='\0';


}
void printx(int num)
{
    int i=0;
    convert(num,16,'x');
    while(x[i]!='\0')
    {
        //write(x[i]);
        //i++;
        buffer[index]=x[i];
        index++;
        i++;
    }
}

void printd(int num)
{
    int i=0;
    convert(num,10,'d');
    while(d[i]!='\0')
    {
        //write(d[i]);
        buffer[index]=d[i];
        index++;
        i++;
    }
}
void prints(char *fmt)
{
    int i=0;
    if(fmt=='\0')
        return;
    while(fmt[i]!='\0')
    {
        //write(fmt[i]);
        buffer[index]=fmt[i];
        index++;
        i++;
    }
}


void printp(unsigned long p)
{
    int i=0;
    prints("0x");
    convert(p,16,'x');
    while(x[i]!='\0')
    {
        //write(x[i]);
        buffer[index]=x[i];
        index++;
        i++;
    }
}

void printc(char fmt )
{
    //write(fmt);
    buffer[index]=fmt;
    index++;
}
/*void newline()
{
    video=video+(80-xp)*2;
    xp=0;
    if(yp<22)
    {
        yp++;
    }
    else
    {
        scroll();
    }
}
void car_return()
{
    video=video-xp*2;
    xp=0;
}*/
void printf(const char *fmt, ...)
{
    va_start(args,fmt);
    int i=0;
    while(fmt[i]!='\0')
    {
        if(fmt[i]=='%')
        {
            i++;
            if(fmt[i]=='x')
                printx(va_arg(args,int));
            else if (fmt[i]=='s')
                prints(va_arg(args,char *));
            else if (fmt[i]=='c')
                printc(va_arg(args,int));
            else if (fmt[i]=='p')
                printp(va_arg(args,unsigned long));
            else if (fmt[i]=='d')
                printd(va_arg(args,int));
        }

        else if(fmt[i]=='\n')
        {
            //newline();
            buffer[index]=fmt[i];
            index++;
        }
        /*else if(fmt[i]=='\r')
        {
            car_return();
        }*/
            //k=(int)fmt[i];
            //printf("\n%d=",k);
        else
            printc(fmt[i]);
        i++;
    }
    va_end(args);
    buffer[index]='\0';
    index=0;
    put_s(buffer);
}
