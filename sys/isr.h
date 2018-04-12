#ifndef ISR_H
#define ISR_H

#include <sys/defs.h>

// structure refered from http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
// extern void isr0();



typedef void (*isr_t)(registers_t);
void isr_install();
void irq_handler(registers_t r);
void irq_handler1(registers_t* r);
void irq_handler2(registers_t* r);
void register_interrupt_handler(uint16_t n, isr_t handler);
uint64_t scanfstdin(uint64_t address, int count);

#endif
