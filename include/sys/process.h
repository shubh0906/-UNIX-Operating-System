#ifndef PROCESS_H
#define PROCESS_H

#define  New            0
#define Ready			1
#define Running		2
#define Zombie			3

#define VM_READ		        4
#define VM_WRITE	       	2
#define VM_EXEC		        1
#define MAX_FD				30

enum vma_types {

	TEXT,
	DATA,
	STACK,
	HEAP,
	NOTYPE
};

typedef struct vma_struct vma_struct;
typedef struct mm_struct mm_struct;
struct file{
	uint64_t   start;
	uint64_t   pgoff;
	uint64_t   size;
	uint64_t   bss_size;
};
struct pcb{
	uint64_t pid;
	uint64_t ppid;
	uint64_t kstack;
	mm_struct* mm;
	uint64_t init_kern;
	uint64_t rsp;			
	uint64_t rip;			
	uint64_t process_state;            
	uint64_t sleep;	
	uint64_t alarm;		
	uint64_t cr3;
	char process_name[15];
	uint64_t fd[MAX_FD];		
	uint64_t wait_on_childpid; 	        
	uint32_t num_child;                      
    char cur_dir[30];
    	//file_t *cur_node;
    	struct pcb* next;
};
struct mm_struct
{
	vma_struct *mmap,*current;
	uint64_t start_stack,mmap_base,brk,start_brk,end_data,start_date,end_code,start_code;
	uint64_t vma_count;	
};
struct vma_struct
{
	mm_struct *mm;
	uint64_t start,end,flag,type;
	vma_struct *next;
	struct file* file;
};
/*struct fd {
        uint64_t current;
        uint64_t permission;
        uint64_t inode_no;
        struct fnode* node;
};*/
typedef struct {
    char name[100];
    int size;
    //int typeflag;
    uint64_t addr_hdr;
    int par_ind;
} fd;
void kernProcess();
void* userInitProcess();
void mapProcessID();
void init_func();
void sleep(uint64_t seconds);
void contextSwitch(struct pcb *next,struct pcb *prev);
uint64_t getPID();
void schedule();
void switchToRing3(struct pcb * init);

void * copy_task_struct(struct pcb* child, struct pcb* parent);
#endif
