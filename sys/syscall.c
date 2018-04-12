#include <sys/defs.h>
#include <sys/mmu.h>
#include <dirent.h>
#include <sys/process.h>
#include <sys/page.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <sys/syscall.h>
#include <sys/string.h>
#include <sys/elf64.h>
#include "isr.h"
#include "common.h"


extern struct pcb *current,*zombie,*waiting;

extern struct pcb *first;

extern fnode root[512];
extern int t_index;
extern int sec;
extern uint64_t read_rip();
int childFlag=0;

uint64_t syscall(uint64_t sysc,uint64_t arg1,uint64_t arg2,uint64_t arg3){

	if (sysc == 1){ 
		return (uint64_t)exitSyscall((int)arg1);
	}
	else if (sysc==2)
	{ 
		return forkSyscall();
	}
	
	else if (sysc == 3)
	{  
		return (uint64_t)readSyscall((long)arg1,arg2,(long)arg3);
	}
	else if (sysc == 4)
	{  
		return (uint64_t)writeSyscall((long)arg1,(long)arg2,(long)arg3);
	}
	else if (sysc == 5){

		uint64_t ret = (uint64_t)openSyscall((char*)arg1,arg2);
		return ret;
	}
    else if (sysc == 7)
    { 
		return (uint64_t)waitpidSyscall((long)arg1,(int)arg2,(int)arg3);
	}
	else if(sysc==11){  
		return execveSyscall((char*)arg1,arg2,arg3);
	}
	else if (sysc == 12){ 
        return chdirSyscall((char*)arg1);
    }
    else if (sysc == 78){
		return clearscreenSyscall();
	}
    else if(sysc == 90)
    {
    	return mallocSyscall(arg1);
    }
    else if(sysc==158)
    {
    	yieldSyscall();
    }
    else if (sysc == 183){
        return getpwdSyscall(arg1);
    }
	else if (sysc == 170)
	{  //clearscreen
		clearScreen();
	}
	else if (sysc==171)
	{
		listprocess();
	}
	else if (sysc == 220){  
		return (uint64_t)readdirSyscall(arg1);
	}
	return 0;
}

/*return type opendirSyscall(uint64_t path){

    char* firstToken = strtok(1,path,'/');
    if (firstToken){
        if (!strcmp(firstToken,"..")){

        }
        else if (!strcmp(firstToken,".")){

        }

    }


}*/
void listprocess()
{
	int count=0;
	kprintf("\n Number |PID    |PPID    |STATE    |NAME");
	struct pcb* start=current;
	while(start->next!=current)
	{
		kprintf("\n %d  |%d    |%d   |%s     |%s", count,start->pid,start->ppid,start->process_state,start->process_name);
		start=start->next;
		count++;
	}
}
uint64_t exitSyscall(int code){
	current->process_state=Zombie;
	struct pcb *temp=current->next;
	while(temp->next!=current)
		temp=temp->next;
	temp->next=current->next;
		
	if(zombie==NULL)
		zombie=current;
	else
	{
		zombie->next=current;
	}
	yieldSyscall();
	return 0;
}
uint64_t clearscreenSyscall(){
	clearScreen();
	return 0;
}
uint64_t execveSyscall(char * file, uint64_t str, uint64_t count){

		//char * file=(char*)filename;
		char** argv = (char**)str;
		//char** env = (char**)count;
		char userstack[10][50];


		uint64_t stackindex=0;
		//int argvindex=0;
		struct pcb* newprocess = (struct pcb*)kmalloc();
		void *kernelstack=(void*)kmalloc();
		mm_struct *mm = (mm_struct *)kmalloc();
		//struct PML4E* currentCR3 = (struct PML4E*)getCR3();

		newprocess->init_kern = newprocess->kstack= ((uint64_t)kernelstack + 4096 - 16);
		newprocess->cr3=(uint64_t)setUserSpace();
		newprocess->mm = mm;

		newprocess->pid=current->pid;
		newprocess->ppid=current->ppid;
		strcpy(file,newprocess->process_name);
		strcpy(file,(char *)userstack[stackindex]);
		stackindex++;
		if (argv)
		{
			while (strcmp(argv[stackindex],"",0)!=0){
				//userstack[stackindex]=(char*)mallocSyscall(20*sizeof(char));
            	//kprintf("");
				strcpy(argv[stackindex],(char*)userstack[stackindex]);
				stackindex++;
				if(argv[stackindex])
					continue;
			}
		}
		uint64_t curr_CR3 = getCR3();
		loadCR3(newprocess->cr3);
		loadElf(newprocess->process_name,newprocess);
		int a=0;
		char **ans=(char**)mallocSyscall(100*sizeof(char*));
		ans[a]=(char*)mallocSyscall(20*sizeof(char));
		strcpy(userstack[a],(char *)ans[a]);
		a++;

			while (a<stackindex){
				ans[a]=(char*)mallocSyscall(20*sizeof(char));
            	//kprintf("%s\n",userstack[a]);
				strcpy(userstack[a],(char*)ans[a]);
				a++;
			}
		loadCR3(newprocess->cr3);
		//strcpy((char*)stackindex,(char*)userstack[0]);
		// userstack[0]=(uint64_t)stackindex;
		//kprintf("--------%d--------------\n",stackindex);

		uint64_t *ptr = (uint64_t*)newprocess->rsp;

		*(ptr-1)=(uint64_t)stackindex;
		*(ptr-2)=(uint64_t)ans;

		//*(uint64_t*)ptr=(uint64_t*)*ans;	


		//memcpy(ptr, (void *)userstack, sizeof(userstack));

		/*int index=stackindex;
		while(index){
			*(uint64_t*)(ptr - 8*index) = (uint64_t)ptr + (stackindex-index)*64;
			index--;
		}
		ptr = ptr- 8*stackindex;*/
		newprocess->rsp = (uint64_t)ptr-16;

		loadCR3(curr_CR3);

		struct pcb* findprevious= first->next;
		/*while (findprevious->next!=current){
			findprevious=findprevious->next;
		}*/

		findprevious->next=newprocess;
		newprocess->next=current->next;

		current = newprocess;

		strcpy("/rootfs/bin",newprocess->cur_dir);
		newprocess->wait_on_childpid=-1;
		newprocess->process_state=Running;
		set_tss_rsp((void *)newprocess->kstack);

		__asm__ __volatile__ ("cli;""movq %0, %%cr3;"::"r"(newprocess->cr3));
		__asm__ __volatile__ ("mov %0, %%rsp;"::"r"(newprocess->kstack));
		__asm__ __volatile__ ("mov $0x23, %%ax;"::);
		__asm__ __volatile__ ("mov %%ax, %%ds;"::);
		__asm__ __volatile__ ("mov %%ax, %%es;"::);
		__asm__ __volatile__ ("mov %%ax, %%fs;"::);
		__asm__ __volatile__ ("mov %%ax, %%gs;"::);

		__asm__ __volatile__ ("pushq $0x23;"::);
		__asm__ __volatile__ ("movq %0, %%rax;"::"r"(newprocess->rsp));
		__asm__ __volatile__ ("pushq $0x23;"::);
		__asm__ __volatile__ ("pushq %%rax;"::);
		__asm__ __volatile__ ("pushfq;"::);
		__asm__ __volatile__ ("popq %%rax;"::);
		__asm__ __volatile__ ("orq $0x200, %%rax;"::);
		__asm__ __volatile__ ("pushq %%rax;"::);
		__asm__ __volatile__ ("pushq $0x2B;"::);
		__asm__ __volatile__ ("pushq %0;"::"r"(newprocess->rip));
		__asm__ __volatile__ ("movq $0x0, %%rdi;"::);
		__asm__ __volatile__ ("movq $0x0, %%rsi;"::);
		//kprintf("%x",getCR3());
		__asm__ __volatile__ ("iretq"::);

		return -1;

}
uint64_t mallocSyscall(uint64_t size)
{
	struct mm_struct *mm = current->mm;
	uint64_t addr = mm->brk;
	mm->brk +=size;
	mm->end_data +=size;
	return addr;
}
int forkSyscall()
{
	struct pcb *temp=NULL;
	struct pcb *parent=current;
	struct pcb *child=	(struct pcb*)kmalloc();
	child->pid = getPID();
	child->ppid = parent->pid;
	//child->rip = (uint64_t)&init_func;
	child->process_state = New;
	child->sleep = 0;
	child->alarm = 0;
	child->wait_on_childpid = -1;
	strcpy(parent->process_name,child->process_name);
	strcpy(parent->cur_dir,child->cur_dir);
	child->num_child = 0;
	child->cr3 =(uint64_t)allocatePage();
	parent->num_child++;
	temp = current->next;
	current->next = child;
	child->next = temp;
	
	struct PDPE *p_pdpe;
    struct PDE *p_pde;
    struct PTE *p_pte;
    struct PDPE *c_pdpe;
    struct PDE *c_pde;
    struct PTE *c_pte;

	struct PML4E *p_pml4e = (struct PML4E*) (0xFFFFFFFF80000000UL | getCR3());
	struct PML4E *c_pml4e = (struct PML4E*) (child->cr3| 0xffffffff80000000UL);

	for(int pml4e_i=0;pml4e_i<510;pml4e_i++)
	{
        uint64_t pml4e_entry=p_pml4e->entries[pml4e_i];
		if (pml4e_entry & P_BIT)   
		{
			c_pdpe=(struct PDPE*)pdpeAlloc(c_pml4e,pml4e_i,'V');
			p_pdpe=(struct PDPE*)((pml4e_entry & 0xFFFFFFFFFFFFF000)| 0xffffffff80000000UL);
			for(int pdpe_i=0;pdpe_i<512;pdpe_i++)
			{
				uint64_t pdpe_entry=p_pdpe->entries[pdpe_i];
        		if (pdpe_entry & P_BIT)
        		{
        			c_pde=(struct PDE*)pdeAlloc(c_pdpe,pdpe_i,'V');
        			p_pde=(struct PDE*)((pdpe_entry & 0xFFFFFFFFFFFFF000)|0xffffffff80000000UL);
        			for(int pde_i=0;pde_i<512;pde_i++)
        			{
        				uint64_t pde_entry=p_pde->entries[pde_i];
        				if (pde_entry & P_BIT)
        				{
        					c_pte=(struct PTE*)pteAlloc(c_pde,pde_i,'V');
        					p_pte=(struct PTE*)((pde_entry & 0xFFFFFFFFFFFFF000)| 0xffffffff80000000UL);
        					for(int pte_i=0;pte_i<512;pte_i++)
        					{
        						uint64_t entry=p_pte->entries[pte_i];
        						if(entry &P_BIT)
        						{
        							uint64_t page=(uint64_t)(entry& 0xFFFFFFFFFFFFF000);                              
									entry=page|P_BIT|U_BIT|COW_BIT;	
									c_pte->entries[pte_i]=entry;
									p_pte->entries[pte_i] = entry;
									kprintf("");	
        						}
        					}
        
        				}
        			}
        
        		}
			}
        
		} 	
	}
	c_pml4e->entries[511]=p_pml4e->entries[511];
	c_pml4e->entries[510]=p_pml4e->entries[510];
	
	loadCR3(child->cr3);
	for(int i=0;i<MAX_FD;i++)
	{
		child->fd[i]=parent->fd[i];
	}
	mm_struct *mm = (mm_struct *)kmalloc();
	child->mm = mm;
	memcpy((void*) child->mm,(void*)parent->mm,sizeof(mm_struct));
	vma_struct *p_vma = parent->mm->mmap;
	vma_struct *c_vma = NULL;
	int flag=0;
	child->mm->mmap=NULL;
	while(p_vma!=NULL)
	{
		if(c_vma==NULL)
			flag=1;
		c_vma=(vma_struct*)kmalloc();
		if(flag==1)
		{
			child->mm->mmap=c_vma;
			flag=0;
		}
		memcpy(c_vma,p_vma,sizeof(vma_struct));
		if(p_vma->file!=NULL)
		{
			c_vma->file=(struct file*)kmalloc();
			memcpy(c_vma->file,p_vma->file,sizeof(struct file));
		}
		p_vma=p_vma->next;
	}
	if(c_vma!=NULL)
		c_vma->next=NULL;

	//child = (struct pcb *)copy_task_struct(child,parent);
	
	
	void *kstack=(void *)kmalloc();
    child->init_kern=child->kstack=((uint64_t)kstack + 0x1000 - 16);
    memcpy((void *)(child->kstack-0x1000+16),(void *)(parent->kstack-0x1000+16),0x1000-16);
	child->rsp = (uint64_t )USER_STACK_TOP;
	
	loadCR3(parent->cr3);
	
	uint64_t rip=read_rip();
	if(rip!=0)
		child->rip=rip;
	__asm__ __volatile__("movq %%rsp, %0;":"=r"(pos));

	if(current == parent) 
	{
		child->kstack = child->kstack - (parent->kstack - pos);
       	return child->pid;
	}
	else 
	{
			childFlag=1;
       		return 0;
  	}
}
uint64_t waitpidSyscall(uint64_t pid, int status, int options){
	if (pid>0)
	{
		current->wait_on_childpid=pid;
		struct pcb* temp=NULL;
		struct pcb* child=NULL;
		struct pcb* parent=current;
		child=current->next;
		temp=child->next;
		child->next=current;
		
		current->next=temp;
		current=child;
		contextSwitch(child,parent);
		//kprintf("after yield");
		//child->rsp=child->init_kern;
		//switchToRing3(child);
	
	}
	return 0;
}

int openSyscall(char * path,uint64_t mode)
{
	//kprintf("------%d",mode);
	if (mode==0) {
		int i=1;
		fnode temp;
		int count = 2;
		//kprintf("%s---\n",path);
		while (i < t_index) {
			temp = root[i];
			//kprintf("%s",temp.name);
			if (strlen(temp.name) == 0)
				break;
			if ((strcmp(&(temp.name[0]), path, 0) == 0) && temp.typeflag == FILE) {
				//kprintf("%s", temp.name);
				while ((current->fd[++count] != 0) && count < MAX_FD);
				current->fd[count] = i;
				//kprintf("%d",count);
				if (count >= MAX_FD) {
					kprintf("Process cannot open any more files.Max Limit of %d reached\n", MAX_FD);
					return -1;
				} else
					return count;
			}
			i++;
		}
		if (i == t_index) {
			kprintf("No such file exists.");
			return -1;
		}
		return -1;
	}
	else if (mode==4){
		int i=1;
		fnode temp;
		int len=strlen(path);
		if (len == 1 && path[0]=='/'){
			return 0;
		}
		if(path[len-1]!='/')
		{
			path[len++]='/';
			path[len]='\0';
		}
		//kprintf("%s",path);
		while(i<t_index)
		{
			temp=root[i];

			if(strlen(temp.name)==0)
				break;
			if((strcmp(&(temp.name[0]), path,0) == 0)&&temp.typeflag==DIRECTORY)
			{
				//kprintf("%d ret",temp.start);
				return i;
			}
			i++;
		}
		if(i==t_index)
		{
			kprintf("No such directory exists.");
			return -1;
		}
		return -1;
	}
	return -1;
}
/*int sys_opendir(char * path)
{
	int i=1;
	fnode temp;
	int len=strlen(path);
	if(path[len]!='/')
	{
		path[len++]='/';
		path[len]='\0';
	}
	while(i<t_index)
	{
		temp=root[i];
		
		if(strlen(temp.name)==0)
			break;
		if((strcmp(&(temp.name[0]), path,0) == 0)&&temp.typeflag==DIRECTORY)
		{
			return temp.start;	
		}
		i++;
	}
	if(i==t_index)
	{
		kprintf("No such directory exists.");
		return -1;
	}
	return -1;
	
}*/
/*int sys_read(int fd,int size,uint64_t buf)
{
	fnode temp=root[current->fd[fd]];
	//kprintf("%s",temp.name);
	if(temp.start==0)
		return 0;
	char* file_start_addr = (char *)(temp.start + 512);
	if(temp.size>size)
		size=temp.size;
	char *buf2=(char *)buf;
	memcpy(buf2,file_start_addr,size);
	buf2[size++]='\0';
	return size;
    
}*/
int readdirSyscall(uint64_t dir)
{
	int parent=dir;
	

		int i=1;
		char *dirName=root[parent].name;
		int len=strlen(dirName);
		kprintf(".     ");
		kprintf("..     ");
		while(i<t_index)
		{
			if(root[i].parent==parent)
				kprintf("%s     ",root[i].name+len);
			i++;
		}
		return 0;
}
int sys_close(int fd)
{
	if(fd<=MAX_FD)
	{
		current->fd[fd]=0;
		return 0;
	}
	else
		return 1;
}
uint64_t chdirSyscall(char* path){

    //opendirSyscall(path);
    int len=strlen(path);
	if(path[len-1]!='/')
	{
		path[len++]='/';
		path[len]='\0';
	}
    if (!isDir(path)){
        kprintf("No such file of directory");
        return 0;
    }
    else{
        memcpy(current->cur_dir,path,strlen((char*)path));
        return 1;
    }
}

/*uint64_t getPIDSyscall()
{
	return current->pid;
}*/
uint64_t getpwdSyscall(uint64_t buffer){
    memcpy((char *)buffer,current->cur_dir,strlen(current->cur_dir));
	return buffer;
}
uint64_t sleepSyscall(int64_t seconds){
	//
	current->sleep=seconds;
	//int cur_sec=sec;
	//kprintf("1");
	__asm__ __volatile("sti");
	//kprintf("2");
	while (current->sleep>0){
		kprintf("");
	}
	//kprintf("after sleep");
	return 0;	
}
uint64_t dup2Syscall(int64_t ofd,int64_t nfd){
	//current->fd[nfd]=current->fd[ofd];
	return nfd;	
}
uint64_t readSyscall(int64_t fd, uint64_t address, int64_t length){

	if (fd==0){ // stdin 0
		return (long)scanfstdin(address,length);
	}
	else{
		fnode temp=root[current->fd[fd]];
		//kprintf("%s",temp.name);
		if(temp.start==0)
			return 0;
		char* file_start_addr = (char *)(temp.start + 512);
		if(temp.size>length)
			length=temp.size;
		char *buf2=(char *)address;
		memcpy(buf2,file_start_addr,length);
		buf2[length++]='\0';
		return length;
	}
	return 0;
}
uint64_t writeSyscall(int64_t fd, int64_t str,int64_t count){
	if (fd==1 || fd==2){			 //stdout 1  stderr 2
		//kprintf("helloqwerty\n");
		kprintf((char*)str);
		return count;
	}
	//kprintf("f off");
	//if (fd==)		Writing in File
	return -1;
}
uint64_t yieldSyscall()
{
	struct pcb* prev=current;
	struct pcb* next=current->next;
	if(next!=NULL)
	{
		current=next;
		contextSwitch(next,prev);
		return 0;
	}	
	return -1;
}
