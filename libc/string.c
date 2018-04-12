#include "../include/string.h"
#include "../include/stdio.h"
#include "../include/printf.h"
#include "../include/unistd.h"

int _strlen_(char *a)
{
  int len=0;
  while(a[len]!='\0')
    len++;
  return len;
}

char* split(int toknum, char* argv1, char separator){

    char **argv2=(char**)sb_malloc(20* sizeof(char*));
    for (int i=0;i<10;i++){
        argv2[i]=(char*)sb_malloc(20* sizeof(char));
    }
    int ss=0,s=0,d=0,d1=0;
    //int flag=0;
    int fflag=0;
    //static char argv2[][];
    //if ( argv1[_strlen_(argv1)-1]!=separator )
    {
        argv1[_strlen_(argv1)]=separator;
        argv1[_strlen_(argv1)]='\0';
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
            fflag=1;
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
    if (fflag==0){
        return argv1;
    }
    if (ss==1 || ss==0){
        ss=2;
    }
    for (int i=ss-1;i<10;i++){
        argv2[i][0]='\0';
    }
    //argv1[_strlen_(argv1)-1]='\0';
    char* ans=argv2[toknum-1];
    if (_strcmp(ans,"",1)){
        char * ans=argv2[toknum-1];
//puts(argv2[toknum]);
        return ans;
    }
    else{
//printf("no token");
        return (void*)0;
    }
//argv2[100][100]={" "};
}


int _strcmp(char *a, char *b,int len)
{
  int alen=_strlen_(a);
  int blen=_strlen_(b);

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
int strncmp(char *a, char *b,int len)
{
  //int alen=_strlen_(a);
  //int blen=_strlen_(b);
  int i=0;
  for(;i<len;i++)
  {
    if(a[i]!=b[i])
      break;
  }
  if(i==len)
    return 1;
  else
    return -1;

}
void _strcpy_(char* src,char* dest)
{
  int i=0;
  while(src[i]!='\0')
  {
    dest[i]=src[i];
    i++;
  }
  dest[i]='\0'; 
}
int _strstr(char *str,char *sub)
{
        int i=0,j=0;
        int slen=_strlen_(str);
        int sublen=_strlen_(sub);
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
void memset(void * ptr,int size)
{
        for(int i=0;i<size;i++)
                ((char*)(ptr))[i]='\0';
}
