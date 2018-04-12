// refer from http://wiki.osdev.org/USTAR

#include <sys/defs.h>
#include <sys/string.h>
#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <dirent.h>
#include <sys/mmu.h>

fnode root[512];
int t_index=1;
int oct2bin(char * size)
{
	int x=0,k=1;
	for(int j=10;j>=0;j--) 
	{
            x = x + (size[j]-'0') * k;
	    k=k*8;
	}
	return x;
}
    
void parse(char *path, int type, uint64_t start, uint64_t size)
{

    int j=1;
    char name[100];
    int len = strlen(path);
    strcpy(path,&name[0]);
    if(start==FILE)
        len--;
    else
        len-=2;
    while(name[len]!='/')
    {
        len--;
        if(len==0)
        {
            fnode temp;
            makeNode(&temp, 0, path, start,size,type);
            root[t_index++]=  temp;
            return;
        }
    }
    name[++len]='\0';
    while((strcmp(&name[0], &(root[j].name[0]),0)==-1) && j<t_index)
    {
        j++;
    }
    if(j<t_index)
     {
        fnode temp;
        makeNode(&temp, j, path, start,size,type);
        root[t_index++]=temp;
     }   
}
void * getFile(char *filename)
{
    struct posix_header_ustar *start = (struct posix_header_ustar *)&_binary_tarfs_start;
        int size=0;
        while (strcmp(start->magic, "ustar", 0)==0)
        {
                //kprintf("name %s %s",start->name,start->typeflag);
                size=oct2bin(start->size);
                //kprintf("name %s %s %d",start->name,start->typeflag,size);
                if(size%512!=0)
                {
                        size=((size/512)*512)+512;
                }
                if (strcmp(start->name,filename,0) == 0)
                {
                        return  (void *)(start+ 1);
                }
        start=(struct posix_header_ustar *)((uint64_t)start+512+size);
         }   
    return NULL;
}
void * isFile(char *filename)
{
    struct posix_header_ustar *start = (struct posix_header_ustar *)&_binary_tarfs_start;
        int size=0;
        while (strcmp(start->magic, "ustar", 0)==0)
        {
                //kprintf("name %s %s",start->name,start->typeflag);
                size=oct2bin(start->size);
                if(size%512!=0)
                {
                        size=((size/512)*512)+512;
                }
                if (strcmp(start->name,filename,0) == 0)
                {
                        if (strcmp(start->typeflag,"0",0)==0)  
                        return  (void *)(start+ 1);
                }
                start=(struct posix_header_ustar *)((uint64_t)start+512+size);
         }   
    return NULL;
}
void * isDir(char *filename)
{
    //kprintf("%s\n",filename);
    struct posix_header_ustar *start = (struct posix_header_ustar *)&_binary_tarfs_start;
        int size=0;
        while (strcmp(start->magic, "ustar", 0)==0)
        {
                //kprintf("name %s %s\n",start->name,start->typeflag);
                size=oct2bin(start->size);
                if(size%512!=0)
                {
                        size=((size/512)*512)+512;
                }
                if (strcmp(start->name,filename,0) == 0)
                {
                        //kprintf("in");
                        if (strcmp(start->typeflag,"5",0)==0) 
                        return  (void *)(start+ 1);
                }
                start=(struct posix_header_ustar *)((uint64_t)start+512+size);
         }   
    return NULL;
}
void init_tarfs()
{
	struct posix_header_ustar *start = (struct posix_header_ustar *)&_binary_tarfs_start;
	int size=0;
	while (strcmp(start->magic, "ustar", 0)==0)
	{
		
		size=oct2bin(start->size);
        //kprintf("name %s %s\n",start->name,start->typeflag);
		
		if (strcmp(start->typeflag,"5",0) == 0)
		{
			parse(start->name, DIRECTORY, (uint64_t)(start), 0);
		//	kprintf("   it is a Directory\n");
		}
		else
		{
			parse(start->name, FILE, (uint64_t)(start),size);
			//kprintf("   it is a File\n");
		}
        if(size%512!=0)
        {
            size=((size/512)*512)+512;
        }
		//kprintf("size %d",size);
		start=(struct posix_header_ustar *)((uint64_t)start+512+size);
	}
}

void makeNode(fnode *node,int parent, char *name, uint64_t start, uint64_t size, uint64_t type)
{
    strcpy(name,node->name);
    node->start = start;
    node->size   = size;
    node->start  = start;
    node->typeflag  = type;
    node->parent = parent;
}
