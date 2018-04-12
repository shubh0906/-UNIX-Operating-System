#ifndef _PAGE_H
#define _PAGE_H
#define PAGE_SIZE	0x1000

#include <sys/defs.h>

volatile uint64_t pos;
uint64_t init_page(uint64_t size);

uint64_t unsetPage();
void* allocatePage();
void deallocatePage(void* address);
void setPage(uint64_t index);
uint64_t init_pageTable(uint64_t physbase, uint64_t physfree, uint64_t base, uint64_t max_length);
void pageFault(registers_t *regs);
uint64_t copyPagetable(uint64_t cPML4E,uint64_t pPML4E);
#endif

