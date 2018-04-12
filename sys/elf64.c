// refer from http://www.cirosantilli.com/elf-hello-world/

#include <sys/defs.h>
#include <sys/string.h>
#include <dirent.h>
#include <sys/tarfs.h>
#include <sys/process.h>
#include <sys/kprintf.h>
#include <sys/mmu.h>
#include <sys/elf64.h>
#include <sys/page.h>

void* loadElf(char * filename, struct pcb *task)
{
	
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)getFile(filename);

	Elf64_Phdr *phdr;
	 if (ehdr->e_ident[1] == 'E' && ehdr->e_ident[2] == 'L' && ehdr->e_ident[3] == 'F')	
		phdr = (Elf64_Phdr *)((uint64_t)ehdr + ehdr->e_phoff);
	else
	 	return NULL;
	mm_struct *mm=task->mm;
	task->rip=ehdr->e_entry;
	task->mm->mmap=NULL;
	for(int i=0; i<ehdr->e_phnum;i++)
	{
		if(phdr->p_type==1) {
			vma_struct *vma = (vma_struct *) kmalloc();
			vma->start = phdr->p_vaddr;
			vma->end = vma->start + phdr->p_memsz;
			vma->flag = phdr->p_flags;
			if (mm->mmap == NULL)
				mm->mmap = vma;
			else
				mm->current->next = vma;
			vma->mm = mm;
			mm->current = vma;
			mm->vma_count++;
			uint64_t size =(((vma->end/PAGE_SIZE)*PAGE_SIZE+PAGE_SIZE)-((vma->start/PAGE_SIZE)*PAGE_SIZE))/PAGE_SIZE;
			uint64_t start =(phdr->p_vaddr/PAGE_SIZE)*PAGE_SIZE;
			while (size > 0)
			{
				uint64_t page = (uint64_t) allocatePage();
				map_v_p2(start, page);
				//kprintf("    %x     ",start);
				size--;
				start += PAGE_SIZE;
			}
			if ((phdr->p_flags == 6) || (phdr->p_flags == 5))
			{
				mm->start_code = phdr->p_vaddr;
				mm->end_code = phdr->p_vaddr + phdr->p_memsz;
				//getCR3();
				memcpy((void *) vma->start, (void *) ((uint64_t) ehdr + phdr->p_offset), phdr->p_filesz);
			}
			vma->file = (struct file *) kmalloc();
			vma->file->start = (uint64_t) ehdr;
			vma->file->pgoff = phdr->p_offset;
			vma->file->size = phdr->p_filesz;
			if (phdr->p_flags == 5)
			{
				vma->type = TEXT;
				vma->file->bss_size = 0;
			}
			else if (phdr->p_flags == 6)
			{
				vma->type = DATA;
				vma->file->bss_size=phdr->p_memsz-phdr->p_filesz;
			}
			else
				vma->type=NOTYPE;
		}
		phdr++;
	}
	vma_struct *stack_vma=(vma_struct*)kmalloc();
	if(mm->mmap==NULL)
		mm->mmap=stack_vma;
	else
		mm->current->next=stack_vma;

	mm->current->next=stack_vma;
	mm->current=stack_vma;
	uint64_t *stack = (uint64_t *)USER_STACK_TOP;
	map_v_p2(USER_STACK_TOP,(uint64_t)allocatePage());
	//uint64_t p=physicalAddress(USER_STACK_TOP);
	//kprintf("in elf64 %x and %x ",USER_STACK_TOP,p);
	stack_vma->start = (uint64_t)stack + PAGE_SIZE;
	stack_vma->end = (uint64_t)stack;
	stack_vma->flag = 0x5;
	stack_vma->type = STACK;
	stack_vma->file = NULL;
	stack_vma->next = NULL;

	task->rsp = (uint64_t)((uint64_t)stack + 4096 - 16);

	mm->vma_count++;

	vma_struct *heap=(vma_struct *)kmalloc();
	mm->current->next=heap;
	mm->current=heap;
	heap->mm=mm;

	heap->start=HEAP_START;
	mm->brk=HEAP_START;
	heap->end=HEAP_START+PAGE_SIZE;
	heap->flag=0x5;
	heap->type=HEAP;
	heap->file=NULL;
	heap->next=NULL;
	map_v_p2(HEAP_START,(uint64_t)allocatePage());
	mm->vma_count++;
	return NULL;
}
