#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/mmu.h>
#include <sys/process.h>
#include <sys/kprintf.h>
#include <sys/page.h>
#include <sys/tarfs.h>
#include <sys/syscall.h>
#include <sys/string.h>
#include <sys/ahci.h>
#include "timer.h"
#include "enumpci.h"
#include "isr.h"
#include "common.h"

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;
extern uint64_t read_rip();
extern int xp,yp;
extern char* video;
void start(uint32_t *modulep, void *physbase, void *physfree)
{
  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;

 uint64_t base=0,max_length=0,limit=0;		
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
    // kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
   	if(smap->length>max_length)
     	 {
        	base=(uint64_t)smap->base;
		//kprintf("bse %p\n",base);
        	limit=(uint64_t)smap->base+smap->length;
		// kprintf("lim %p\n",limit);
		max_length=smap->length;
     	 }  
    }
    }
//memset((void*)(0xB8000),0,25160);
//max_length=max_length+1;
uint64_t new_physfree=init_pageTable((uint64_t)physbase, (uint64_t)physfree, base,limit);
kprintf("physfree %p\n", new_physfree);
//__asm__ __volatile__ ("hlt");
kernmemMap((uint64_t)physbase,new_physfree);
phy2vir(limit);
mapProcessID();
kernProcess();
init_tarfs();
/*char *str="bin/2/1/a.txt";
int d=sys_open(str);
uint64_t addr=(uint64_t)kmalloc();
int bytes=sys_read(d,500,addr);
kprintf("\n%d         %d\n %s",d,bytes,addr);*/
//char *str="bin/2";
//uint64_t d=sys_opendir(str);
//uint64_t addr=(uint64_t)kmalloc();
//int bytes=sys_readdir(d);
//kprintf("\n%d         %d\n",d,bytes);
kprintf("physbase %p\n", (uint64_t)physbase);
kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
clearScreen();
printCursor();
yp=0;
video=((char*)0xffffffff800B8000);
kprintf("             -----------Welcome to SBUnix------------");
video=((char*)0xffffffff800B8000)+160;
yp=1;
xp=0;

//kprintf("%x",read_rip);
struct pcb* init=(struct pcb*)userInitProcess("bin/sbush");
//userInitProcess("bin/sbush");
init_timer(1);
key();
/*int64_t cba=(int64_t)kmalloc();
char* abc="hello";
strcpy(abc,(char*)cba);
writeSyscall(1,cba,5);*/
//init_timer(1);
//key();

    //char* abc="hello";

//uint64_t aaa=scanfstdin((uint64_t)abc,5);
    //kprintf("%s",aaa);
//if (aaa){}
/*uint64_t *ptr=(uint64_t*)((uint64_t)0x4000000UL);
kprintf("%p",*ptr);*/
switchToRing3(init);
//init_func();
//	for(int i=0;i<50;i++)
//		kprintf("%d\n",i);
  kprintf("physbase %p\n", (uint64_t)physbase);
kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
//init_timer(1);

//walk();
while(1);
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *temp1, *temp2;

  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__(
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );
  init_gdt();
  isr_install();
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
  for(
    temp1 = "", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;
  while(1);		
	//init_gdt();
	//init_timer(100);
}
