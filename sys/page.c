#include <sys/defs.h>
#include "isr.h"
#include <sys/page.h>
#include <sys/kprintf.h>
#include <sys/mmu.h>
#include <sys/process.h>
#include <sys/elf64.h>
#include "isr.h"
#include <sys/string.h>
uint64_t *pageMap;
uint64_t firstFreepage;
uint64_t lastPage;

extern struct pcb *current;
uint64_t init_page(uint64_t size){
	uint64_t start=0,*startPage;
	startPage=pageMap;
	while (start!=size){
		*startPage++=start+1;
		start++;
	}
	kprintf("SP %x\n",startPage);
	pageMap[size]=-1;
	return (uint64_t)startPage;
}
uint64_t unsetPage(){
	if(pageMap[firstFreepage]==-1)
		return -1;
	uint64_t l=firstFreepage;
	firstFreepage=pageMap[l];
	pageMap[l]=0;
	return l;
}
void* allocatePage(){
	uint64_t a =unsetPage();
	if(a==-1)
		return NULL;
	//memset((void*)(a*PAGE_SIZE),0,PAGE_SIZE);
	//for(int i=0;i<PAGE_SIZE;i++)
	//	((char*)(a*PAGE_SIZE))[i]=0;
//	kprintf("all %p",a*PAGE_SIZE);
	return (void*)(a*PAGE_SIZE);
}

void deallocatePage(void* address){
        uint64_t index = (uint64_t)address/PAGE_SIZE;
	setPage(index);
}

void setPage(uint64_t index){
	if (index<firstFreepage){
		pageMap[index]=firstFreepage;
		firstFreepage=index;
	}
	else if (index>firstFreepage){
		uint64_t a=firstFreepage;
		while (pageMap[a]<index){
			a++;
		}
		uint64_t b=pageMap[a];
		pageMap[a]=index;
		pageMap[index]=b;
	}
}
uint64_t init_pageTable(uint64_t physbase, uint64_t physfree, uint64_t base, uint64_t max_length)
{
	uint64_t i=0;
	lastPage=max_length/PAGE_SIZE;
	pageMap=(uint64_t *)physfree;
	physfree=init_page(lastPage);
	physfree=((physfree/0x1000)+1)*0x1000;
	firstFreepage=0;
	for(i=0;i<physfree/PAGE_SIZE;i++)
		unsetPage();
	kprintf("%d",i);
	for(i=(base/PAGE_SIZE);i<physbase/PAGE_SIZE;i++)
		setPage(i);
	//kprintf("%d",firstFreepage);
	return physfree;	
}
void pageFault(registers_t *regs)
{
	//kprintf("PF---");
	uint64_t err_code = regs->err_code & 0xF;
	uint64_t fault_addr;
	int fault = 0;
	__asm__ __volatile__ ("mov %%cr2, %0;":"=r"(fault_addr):);
	//kprintf("error code %d fault address %x pid %d",err_code,fault_addr,current->pid);
	if((err_code & P_BIT) && (err_code & W_BIT))
	{
		uint64_t p_fault=physicalAddress(fault_addr);
		
		if(p_fault & COW_BIT)
		{
			//kprintf("COW\n",err_code,fault_addr);
		
			uint64_t vir_addr=(uint64_t)kmalloc();
			uint64_t p_addr=physicalAddress(vir_addr& 0xFFFFFFFFFFFFF000);
			uint64_t v = (fault_addr&(0xffffffffffffffffUL<<12));
			memcpy((void*)vir_addr,(void*)v,PAGE_SIZE);
			map_v_p2(v,p_addr);
			loadCR3(current->cr3);
			return;
		}
	}
	else if(!(err_code &P_BIT))
		fault=1;
	vma_struct* vma=NULL;
	if(current->mm->mmap)
		vma = current->mm->mmap;
	uint64_t start, end;
	while(vma!=NULL)
	{
		start = vma->start;
		end = vma->end;
		if(vma->type==HEAP&& fault_addr>=start && fault_addr<=HEAP_END)
		{
			//kprintf("HEAP\n");
			while(fault_addr<end)
				end=end-PAGE_SIZE;
			uint64_t *heap=(uint64_t*)end;
			map_v_p2(end,(uint64_t)allocatePage());
			vma->end=((uint64_t)heap)+PAGE_SIZE;
			break;
		}
		if(vma->type==STACK && fault_addr<=end && (start-end-PAGE_SIZE)<=STACK_LIMIT && (fault_addr-end-PAGE_SIZE)<=STACK_LIMIT)
		{
			kprintf("START\n");
			kprintf("%x end, %x start",vma->end,vma->start);
			uint64_t *stack=(uint64_t*)(end-PAGE_SIZE);
			map_v_p2(end,(uint64_t)allocatePage());
			vma->end=((uint64_t)stack);
			break;
		}
		vma=vma->next;
	}
	int fault2=0;
	if(vma==NULL)
		fault2=1;
	if(fault && fault2)
	{
		kprintf("\nSegmentation Fault %p (%p) - Process Terminated", fault_addr, err_code);
		__asm__ __volatile__("hlt");
	}
}
