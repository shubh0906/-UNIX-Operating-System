#include <sys/defs.h>
#include <sys/kprintf.h>
#include "des_table.h"
//#include "isr.h"
#include "des_table.h"
#define max 256

void init_idt(){
        idt_ptr.limit=sizeof(idt_entry_t)*max -1;
        idt_ptr.base=(uint64_t)&idt_entry;
	__asm__ __volatile__("lidtq (%0)" : : "r" (&idt_ptr));

    /*idt_gate(0,(uint64_t)&isr0,0x08); //read
    idt_gate(1,(uint64_t)&isr1,0x08); //write
    idt_gate(2,(uint64_t)&isr2,0x08); //sleep
    idt_gate(3,(uint64_t)&isr3,0x08); //dup2
    idt_gate(4,(uint64_t)&isr4,0x08); //exit
    idt_gate(5,(uint64_t)&isr5,0x08); //waitpid */

    idt_gate(14,(uint64_t)&isr14,0x08, 0x8E); //page fault
        idt_gate(32,(uint64_t)&timer_routine, 0x08, 0x8E);
	idt_gate(33,(uint64_t)&keyboard_routine, 0x08, 0x8E);
	idt_gate(128,(uint64_t)&syscall_routine, 0x08, 0xEE);
}
void idt_gate(uint8_t index, uint64_t base, uint16_t sel, uint16_t typeAttribute){
//kprintf("qwerty");
        idt_entry[index].base_lo= base & 0xFFFF;
	idt_entry[index].base_m= (base>>16) & 0xFFFF;
        idt_entry[index].base_hi = (base>>32) & 0xFFFFFFFF;
        idt_entry[index].sel=sel;
        idt_entry[index].always0=0;
        idt_entry[index].flags=typeAttribute;
	idt_entry[index].zeros=0;
}

