#include <sys/defs.h>
#include <sys/mmu.h>
#include <sys/process.h>
#include <sys/page.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/elf64.h>
#define MAX_PROCESSES 100
uint32_t pID[MAX_PROCESSES];

struct pcb *first,*current,*last,*zombie=NULL,*waiting=NULL;
extern int sec;

uint64_t getPID()
{
	uint64_t i = 0;
	while (i < MAX_PROCESSES){
		if(pID[i] == 0) {
			pID[i] = 1;
			return i;
		}
		i+=1;
	}
	return -1;
}

void mapProcessID()
{       
        uint64_t i = 0;
        while(i < MAX_PROCESSES){
                pID[i] = 0;
                i+=1;
        }
}

void kernProcess(){
	struct pcb *init= (struct pcb*)kmalloc();
	init->pid = getPID();
	init->ppid = 0;
	//init->mm = NULL;
	init->rip = (uint64_t)&init_func;
	init->process_state = Running;
	init->sleep = 0;
	init->alarm = 0;
	init->wait_on_childpid = -1;
	init->cr3 = (uint64_t)getCR3();
	strcpy("Kernel",init->process_name);
	init->num_child = 0;
	first = init;
	void *kstack = (void *)kmalloc();
	init->rsp = init->init_kern = init->kstack = ((uint64_t)kstack +0x1000 - 16);
	init->next=init;
	current = init;
}

void* userInitProcess(char *filename){
	struct pcb *init= (struct pcb*)kmalloc();
	init->pid = getPID();
	init->ppid = 0;
	init->rip = (uint64_t)&init_func;
	init->process_state = New;
	init->sleep = 0;
	init->alarm = 0;
	init->wait_on_childpid = -1;
	strcpy(filename,init->process_name);
	strcpy("/rootfs/bin/",init->cur_dir);
	init->num_child = 0;

	init->cr3 =(uint64_t)setUserSpace();
	uint64_t prevCR3=getCR3();
	loadCR3(init->cr3);
	mm_struct *mm = (mm_struct *)kmalloc();
	init->mm = mm;
	init->mm->vma_count=0;
	void *kstack1 = (void *)kmalloc();
	init->init_kern=init->kstack=((uint64_t)kstack1 +0x1000-16);
	loadElf(filename,init);
	struct pcb *temp =  (struct pcb*)current;
	while(temp->next!=current)
	temp=temp->next;
	temp->next=init;
	init->next=current;
	loadCR3(prevCR3);
	return (void*)init;
}


void init_func()
{
	while(1)
		schedule();
}
void schedule()
{
	kprintf("Thread %d",current->pid);
	struct pcb *prev =  (struct pcb*)current;
        if(current!=last)
	current = current->next;
	else
	 current=first;
        contextSwitch(current,prev);

}

void sleep(uint64_t seconds){
	current->sleep=seconds;
	int cur_sec=sec;
	while (sec - cur_sec < current->sleep){
		int temp=sec;
		while(sec==temp);
	}
}

void switchToRing3(struct pcb * init){
	//flush_tss();
	set_tss_rsp((void *)init->kstack);

	current=init;

	__asm__ __volatile__ ("cli;""movq %0, %%cr3;"::"r"(init->cr3));
	__asm__ __volatile__ ("mov $0x23, %%ax;"::);
	__asm__ __volatile__ ("mov %%ax, %%ds;"::);
	__asm__ __volatile__ ("mov %%ax, %%es;"::);
	__asm__ __volatile__ ("mov %%ax, %%fs;"::);
	__asm__ __volatile__ ("mov %%ax, %%gs;"::);

	__asm__ __volatile__ ("pushq $0x23;"::);
	__asm__ __volatile__ ("movq %0, %%rax;"::"r"(init->rsp));
	__asm__ __volatile__ ("pushq $0x23;"::);
	__asm__ __volatile__ ("pushq %%rax;"::);
	__asm__ __volatile__ ("pushfq;"::);
	__asm__ __volatile__ ("popq %%rax;"::);
	__asm__ __volatile__ ("orq $0x200, %%rax;"::);
	__asm__ __volatile__ ("pushq %%rax;"::);
	__asm__ __volatile__ ("pushq $0x2B;"::);
    __asm__ __volatile__ ("pushq %0;"::"r"(init->rip));
	__asm__ __volatile__ ("movq $0x0, %%rdi;"::);
	__asm__ __volatile__ ("movq $0x0, %%rsi;"::);
	__asm__ __volatile__ ("iretq"::);


}
void freemm(mm_struct * mm){
	if(mm){
		vma_struct* vma = mm->mmap;
		while(vma){
			vma_struct *free = vma;
			vma = vma->next;
			kfree((uint64_t)free);
		}
	}
	return;
}

void exit(struct pcb* process){
	freemm(process->mm);
	kfree((uint64_t)process->mm);
	process->mm=NULL;
	deletePagetable(process->cr3);
	kfree((uint64_t)process->kstack);
}

void contextSwitch(struct pcb *next, struct pcb *prev) {
	set_tss_rsp((void*) next->kstack);
	next->process_state=Running;
	loadCR3(next->cr3);
	__asm__ __volatile__ ("sti;""movq %%rsp, (%0)"::"r"(&(prev->kstack)));
	__asm__ __volatile__ ("movq %0, %%rsp;"::"r"(next->kstack));
	__asm__ __volatile__ ("movq $1f,%0;":"=g"(prev->rip):);

	__asm__ __volatile__ ("pushq %0;"::"r"(next->rip));
	//kprintf("rip %x, prev %x, next %x,rip2 %x\n",prev->rip,prev,next,next->rip);
	__asm__ __volatile__ ("retq;"	::);
	__asm__ __volatile__("1:\t");
	//kprintf("Hello");
}
