#include "stdio.h"
#include "syscall.h"
#include "sysdef.h"


char get_ch()
{
  _read_(3,0,s,1);
  return s[0];
}
void _gets_(char *str)
{
  
  _read_(3,0,str,1);
}
void put_ch(char str)
{
  p[0]=str;
  _write_(4,1,(long)p,1);
}
void put_s(char *str)
{
  int i=0;
  do
  {
    put_ch(str[i]);
    i++;
  } while(str[i]!='\0');
}
long sb_fopen(char *path)
{
	return _open_(0x05,path,2);	
}

long sb_fread(long fd)
{
	return _read_(0x03,fd,buf,1000);
}
