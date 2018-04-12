#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/string.h>


void memset(void * ptr,int val,int size)
{
        for(int i=0;i<size;i++)
                ((char*)(ptr))[i]=val;
}

int strlen(char *a)
{
  int len=0;
  while(a[len]!='\0')
    len++;
  return len;
}
int strcmp(char *a, char *b,int len)
{
   int alen=strlen(a);
   int blen=strlen(b);

   if(alen==blen)
   {
        //put_s("in");
      while(*(a+len)==*(b+len))
      {
        //put_s("yy");
        if(a[len]=='\0' && b[len]=='\0')
         { break;}
        len++;
      }
      if(len==alen)
        return 0;
   }
   return -1;
}
void strcpy(char* src,char* dest)
{
  int i=0;
  while(src[i]!='\0')
  {
    dest[i]=src[i];
    i++;
  }
  dest[i]='\0';
}
int strstr(char *str,char *sub)
{
        int i=0,j=0;
        int slen=strlen(str);
        int sublen=strlen(sub);
        if(slen>=sublen)
        {
          while(i<(slen-sublen))
          {
            j=0;
            while(str[i+j]==sub[j])
            {
              //printf("%s\n",str[i+j] );
              j++;
              if(sub[j]=='\0')
                return 1;
            }
            i++;
          }
        }
        return 0;

}
void memcpy(void *dest, void *src, uint64_t len)
{
    char *pd = (char *)dest;
    char *ps = (char *)src;
    int i=0;
    while(i<len)
    {
        *pd++=*ps++;
        i++;
    }
}
