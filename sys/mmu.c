#include <sys/defs.h>
#include <sys/mmu.h>
#include <sys/page.h>
#include <sys/kprintf.h>
#include <sys/string.h>
extern char kernmem;
extern char * video;
struct PML4E* pml4e;
uint64_t cr3;

extern uint64_t *pageMap;
uint64_t getIndex(uint64_t address, int shift){
	return (uint64_t)((address>>shift) & 0x1FF);
}
void loadCR3(uint64_t CR3)
{
	__asm volatile("movq %0, %%cr3":: "b"(CR3));
}
uint64_t getCR3(){
uint64_t CR3;
	__asm volatile("mov %%cr3, %0" : "=r" (CR3));
	return CR3;
}
void map_v_p(uint64_t virtual_address, uint64_t physical_address){
	struct PDPE *pdpe;
	struct PDE *pde;
	struct PTE *pte;

	uint64_t pml4e_i=getIndex(virtual_address,39);
	uint64_t pdpe_i=getIndex(virtual_address,30);
	uint64_t pde_i=getIndex(virtual_address,21);
	uint64_t pte_i=getIndex(virtual_address,12);

	uint64_t pml4e_entry=pml4e->entries[pml4e_i];
	if (pml4e_entry & P_BIT)
		pdpe=(struct PDPE*)(pml4e_entry & 0xFFFFFFFFFFFFF000);
	else
		pdpe=(struct PDPE*)pdpeAlloc(pml4e,pml4e_i,'P');

	uint64_t pdpe_entry=pdpe->entries[pdpe_i];
	if (pdpe_entry & P_BIT)
		pde=(struct PDE*)(pdpe_entry & 0xFFFFFFFFFFFFF000);
	else
		pde=(struct PDE*)pdeAlloc(pdpe,pdpe_i,'P');

	uint64_t pde_entry=pde->entries[pde_i];
	if (pde_entry & P_BIT)
		pte=(struct PTE*)(pde_entry & 0xFFFFFFFFFFFFF000);
	else
		pte=(struct PTE*)pteAlloc(pde,pde_i,'P');
	
	uint64_t entry = physical_address;
	entry=entry|(P_BIT|W_BIT);
	pte->entries[pte_i]=entry;
}
void map_v_p2(uint64_t virtual_address, uint64_t physical_address){
        struct PDPE *pdpe;
        struct PDE *pde;
        struct PTE *pte;

        uint64_t pml4e_i=getIndex(virtual_address,39);
        uint64_t pdpe_i=getIndex(virtual_address,30);
        uint64_t pde_i=getIndex(virtual_address,21);
        uint64_t pte_i=getIndex(virtual_address,12);

	struct PML4E *pml4e = (struct PML4E*) getCR3();
	//kprintf("\n%x|%x|",pml4e,pml4e_i);

	pml4e = (struct PML4E*) (0xFFFFFFFF80000000UL | (uint64_t) pml4e); 

        uint64_t pml4e_entry=pml4e->entries[pml4e_i];
        if (pml4e_entry & P_BIT)
                pdpe=(struct PDPE*)((pml4e_entry & 0xFFFFFFFFFFFFF000)| 0xffffffff80000000UL);
        else
                pdpe=(struct PDPE*)pdpeAlloc(pml4e,pml4e_i,'V');
		//kprintf("%x|%x|",pdpe,pdpe_i);         

        uint64_t pdpe_entry=pdpe->entries[pdpe_i];
        if (pdpe_entry & P_BIT)
                pde=(struct PDE*)((pdpe_entry & 0xFFFFFFFFFFFFF000)|0xffffffff80000000UL);
        else
                pde=(struct PDE*)pdeAlloc(pdpe,pdpe_i,'V');
        // kprintf("%x |%x|%x|%x",pde,pde_i,physical_address,virtual_address);

        uint64_t pde_entry=pde->entries[pde_i];
        if (pde_entry & P_BIT)
                pte=(struct PTE*)((pde_entry & 0xFFFFFFFFFFFFF000)| 0xffffffff80000000UL);
        else
                pte=(struct PTE*)pteAlloc(pde,pde_i,'V');

        uint64_t entry=physical_address;
        entry=entry|(P_BIT|U_BIT|W_BIT);
        pte->entries[pte_i]=entry;
		//kprintf("%x %x\n",virtual_address,physical_address);
}

struct PDPE * pdpeAlloc(struct PML4E *pml4e, uint64_t pml4e_i,char from){
	struct PDPE*pdpe=(struct PDPE*)allocatePage();
	struct PDPE* p_pdpe=pdpe;
	if(from=='V')
		pdpe=(struct PDPE*)((uint64_t)pdpe | 0xffffffff80000000UL);
	memset((void*)(pdpe),0,PAGE_SIZE);
	uint64_t pdpe_entry=(uint64_t)p_pdpe;
	if(from=='V')
		pdpe_entry=pdpe_entry|U_BIT;
	pdpe_entry=pdpe_entry|(P_BIT|W_BIT);
	pml4e->entries[pml4e_i]=pdpe_entry;
	//kprintf("1 :%p,%p\n",pml4e_i,pdpe_entry);
	return pdpe;
}

struct PDE * pdeAlloc(struct PDPE *pdpe, uint64_t pdpe_i,char from){
	struct PDE* pde=(struct PDE*)allocatePage();
	struct PDE* p_pde=pde;
	if(from=='V')
		pde=(struct PDE*)((uint64_t)pde | 0xffffffff80000000UL);
	memset((void*)(pde),0,PAGE_SIZE);
	uint64_t pde_entry=(uint64_t)p_pde;
	if(from=='V')
		pde_entry=pde_entry|U_BIT;
	pde_entry=pde_entry|(P_BIT|W_BIT);

	pdpe->entries[pdpe_i]=pde_entry;
	//kprintf("2 :%p,%p\n",pdpe_i,pde_entry);
	return pde;
}


struct PTE * pteAlloc(struct PDE *pde, uint64_t pde_i,char from){
	struct PTE*pte=(struct PTE*)allocatePage();
	struct PTE*p_pte=pte;
	if(from=='V')
		pte=(struct PTE*)((uint64_t)pte | 0xffffffff80000000UL);
	memset((void*)(pte),0,PAGE_SIZE);
	uint64_t pte_entry=(uint64_t)p_pte;
	if(from=='V')
		pte_entry = pte_entry|U_BIT;
	pte_entry=pte_entry|(P_BIT|W_BIT);
	pde->entries[pde_i]=pte_entry;
	//kprintf("3 :%p,%p        ",pde_i,pte_entry);
	return pte;
}

void kernmemMap(uint64_t physbase,uint64_t physfree){
	struct PDPE *pdpe;
	struct PDE *pde;
	struct PTE *pte;
	
	uint64_t virtual_address=(uint64_t)&kernmem;
	
	uint64_t pml4e_i=getIndex(virtual_address,39);
	uint64_t pdpe_i=getIndex(virtual_address,30);
	uint64_t pde_i=getIndex(virtual_address,21);
	
	pml4e=(struct PML4E*)allocatePage();
	memset((void*)(pml4e),0,PAGE_SIZE);
	cr3=(uint64_t)pml4e;
	
	pdpe=pdpeAlloc(pml4e,pml4e_i,'P');
	pde=pdeAlloc(pdpe,pdpe_i,'P');
	pte=pteAlloc(pde,pde_i,'P');
	//int last=0;
	while(physbase<physfree){
		uint64_t pte_i=getIndex(virtual_address,12);
		//kprintf("%p\n",pte_i);
		uint64_t entry=physbase;	
		entry=entry|(P_BIT|W_BIT);
		pte->entries[pte_i]=entry;
		physbase+=0x1000;
		virtual_address+=0x1000;
		//last=pte_i;
	}
//	kprintf("\n\n%p,%d,%p,%p",pte->entries[87],last,physbase,pageMap);
	//__asm volatile("movq %0, %%cr3":: "b"(cr3));
}

void deletePagetable(uint64_t pml4e) {
	struct PML4E *pml4e1 = (struct PML4E *) pml4e;
	int pml4e_i = 0;
	for (pml4e_i = 0; pml4e_i < 510; pml4e_i++) {
		uint64_t pml4e_entry = pml4e1->entries[pml4e_i];
		if (pml4e_entry & P_BIT) {
			struct PDPE *pdpe = (struct PDPE *) (pml4e_entry & 0xFFFFFFFFFFFFF000);
			int pdpe_i = 0;
			for (pdpe_i = 0; pdpe_i < 512; pdpe_i++) {
				uint64_t pdpe_entry = pdpe->entries[pdpe_i];
				if (pdpe_entry & P_BIT) {
					struct PDE *pde = (struct PDE *) (pdpe_entry & 0xFFFFFFFFFFFFF000);
					int pde_i = 0;
					for (pde_i = 0; pde_i < 512; pde_i++) {
						uint64_t pde_entry = pde->entries[pde_i];
						if (pde_entry & P_BIT) {
							struct PTE *pte = (struct PTE *) (pde_entry & 0xFFFFFFFFFFFFF000);
							int pte_i = 0;
							for (pte_i = 0; pte_i < 512; pte_i++) {
								uint64_t pte_entry = pte->entries[pte_i];
								pte_entry = (pte_entry & 0xFFFFFFFFFFFFF000) | P_BIT | U_BIT | W_BIT;
								pte->entries[pte_i] = pde_entry;
							}
						}
					}
				}
			}
		}
	}
}

void phy2vir(uint64_t limit)
{
	uint64_t virtual_address=0xffffffff80000000UL;
	uint64_t phy_address=0x0UL;
	while(phy_address<=limit)
	{
		map_v_p(virtual_address,phy_address);
		phy_address+=0x1000;
		virtual_address+=0x1000;
	}
	map_v_p((uint64_t)0xffffffff800b8000UL,0xb8000);

	pageMap = (uint64_t*)(0xFFFFFFFF80000000UL | (uint64_t) pageMap); 
	video=(char *)(0xFFFFFFFF80000000UL | (uint64_t) video);
	loadCR3(cr3);
//	kprintf("Shubam");
}
void* setUserSpace()
{
	struct PML4E *newPML4E = (struct PML4E *)allocatePage();
	struct PML4E *curPML4E = (struct PML4E *)getCR3();
	curPML4E = (struct PML4E *)((uint64_t)curPML4E | 0xffffffff80000000UL);
	((struct PML4E *)((uint64_t)newPML4E | 0xffffffff80000000UL))->entries[511]= curPML4E->entries[511];
	return (void *)newPML4E;
}
void* kmalloc()
{
	uint64_t p_newPage=(uint64_t)allocatePage();
	uint64_t v_newPage=p_newPage|0xFFFFFFFF80000000UL;
	map_v_p2(v_newPage,p_newPage);
	memset((void*)(v_newPage),0,PAGE_SIZE);
	return (void*) v_newPage;
}
void kfree(uint64_t virtual_address){

	struct PML4E *pml4e=(struct PML4E*)getCR3();
	struct PDPE *pdpe=NULL;
    struct PDE *pde=NULL;
    struct PTE *pte=NULL;

    uint64_t pml4e_i=getIndex(virtual_address,39);
    uint64_t pdpe_i=getIndex(virtual_address,30);
    uint64_t pde_i=getIndex(virtual_address,21);
    uint64_t pte_i=getIndex(virtual_address,12);

    uint64_t pml4e_entry=pml4e->entries[pml4e_i];
    if (pml4e_entry & P_BIT)
            pdpe=(struct PDPE*)(pml4e_entry & 0xFFFFFFFFFFFFF000);

    uint64_t pdpe_entry=pdpe->entries[pdpe_i];
    if (pdpe_entry & P_BIT)
            pde=(struct PDE*)(pdpe_entry & 0xFFFFFFFFFFFFF000);

    uint64_t pde_entry=pde->entries[pde_i];
    if (pde_entry & P_BIT)
            pte=(struct PTE*)(pde_entry & 0xFFFFFFFFFFFFF000);
	uint64_t physical_address=0;
    if (pte){
    	physical_address=pte->entries[pte_i];
    	deallocatePage((void *)physical_address);
    }
}
uint64_t physicalAddress(uint64_t virtual_address)
{
	struct PDPE *pdpe=NULL;
	struct PDE *pde=NULL;
	struct PTE *pte=NULL;

	uint64_t pml4e_i=getIndex(virtual_address,39);
	uint64_t pdpe_i=getIndex(virtual_address,30);
	uint64_t pde_i=getIndex(virtual_address,21);
	uint64_t pte_i=getIndex(virtual_address,12);

	struct PML4E *pml4e = (struct PML4E*) getCR3();

	pml4e = (struct PML4E*) (0xFFFFFFFF80000000UL | (uint64_t) pml4e);

	uint64_t pml4e_entry=pml4e->entries[pml4e_i];
	if (pml4e_entry & P_BIT)
		pdpe=(struct PDPE*)((pml4e_entry & 0xFFFFFFFFFFFFF000)| 0xffffffff80000000UL);

	uint64_t pdpe_entry=pdpe->entries[pdpe_i];
	if (pdpe_entry & P_BIT)
		pde=(struct PDE*)((pdpe_entry & 0xFFFFFFFFFFFFF000)|0xffffffff80000000UL);

	uint64_t pde_entry=pde->entries[pde_i];
	if (pde_entry & P_BIT)
		pte=(struct PTE*)((pde_entry & 0xFFFFFFFFFFFFF000)| 0xffffffff80000000UL);

	uint64_t entry=0;
	if(pte!=NULL)
	entry=pte->entries[pte_i];
	return entry;
	//kprintf("%x %x\n",virtual_address,physical_address);
}