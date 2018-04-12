#ifndef DES_TABLE_H
#define DES_TABLE_H

#include <sys/defs.h>
#define max 256
#define maxgdt 5
void init_des_tables();

struct idt_entry_struct
{
   uint16_t base_lo;             
   uint16_t sel;                 
   uint8_t  always0;    
   uint8_t  flags; 
   uint16_t base_m;             
   uint32_t base_hi;
   uint32_t zeros;   
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
   uint16_t limit;
   uint64_t base;     
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

void idt_gate(uint8_t, uint64_t, uint16_t, uint16_t);
void init_idt();

idt_entry_t idt_entry[max];
idt_ptr_t idt_ptr;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr14();
extern void timer_routine();
extern void keyboard_routine();
extern void syscall_routine();
#endif
