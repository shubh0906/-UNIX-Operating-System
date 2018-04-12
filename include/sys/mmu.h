#ifndef MMU_H
#define MMU_H

#define P_BIT	0x1
#define W_BIT	0x2
#define U_BIT	0x4
#define COW_BIT 0x100
#define PAGE_SIZE_BIT 0x80

void* kmalloc();
void loadCR3(uint64_t CR3);
uint64_t getIndex(uint64_t address,int shift);
uint64_t getCR3();
void kernmemMap(uint64_t physbase,uint64_t physfree);
void map_v_p(uint64_t virtual_address, uint64_t physical_address);
void map_v_p2(uint64_t virtual_address, uint64_t physical_address);
void deletePagetable(uint64_t pml4e);
void phy2vir(uint64_t limit);
void memset(void * ptr,int val,int size);
void* setUserSpace();
struct PML4E{
	uint64_t entries[512];
};

struct PDPE{
	uint64_t entries[512];
};

struct PDE{
	uint64_t entries[512];
};

struct PTE{
	uint64_t entries[512];
};
uint64_t physicalAddress(uint64_t virtual_address);
struct PDPE * pdpeAlloc(struct PML4E *pml4e, uint64_t pml4e_i,char from);
struct PDE * pdeAlloc(struct PDPE *pdpe, uint64_t pdpe_i,char from);
struct PTE * pteAlloc(struct PDE *pde, uint64_t pde_i,char from);
uint64_t vir2phy(uint64_t virtual_address);
void kfree(uint64_t virtual_address);

#endif
