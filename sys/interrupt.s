.global timer_routine
.global keyboard_routine
.global syscall_routine1
.global read_rip
.extern volatile uint64_t pos

.macro PF_interrupt name num
        .globl \name
        .type \name, @function
        \name:
                pushq $\num
                jmp syscall_routine1
.ENDM

.macro SYSCALL_interrupt name num
        .globl \name
        .type \name, @function
        \name:
                pushq $0
                pushq $\num
                jmp syscall_routine1
.ENDM

PF_interrupt isr14 14
SYSCALL_interrupt syscall_routine 128

/*SYSCALL_interrupt isr0 0
SYSCALL_interrupt isr1 1
SYSCALL_interrupt isr2 2
SYSCALL_interrupt isr3 3
SYSCALL_interrupt isr4 4
SYSCALL_interrupt isr5 5*/

timer_routine:
//  cli                
//  pushq byte 0         
//  pushq byte 0         
  jmp irq_common_stub


keyboard_routine:
 cli
 jmp irq_common_stub1

syscall_routine1:
jmp irq_common_stub2

.extern isr_handler
.extern irq_handler1
.extern irq_handler2


read_rip:
  popq %rax
  jmpq %rax 

irq_common_stub2:
    movq %rsp, pos
    pushq %rdi
   	pushq %rax
   	pushq %rbx
   	pushq %rcx
   	pushq %rdx
   	pushq %rbp
   	pushq %rsi
   	pushq %r8
   	pushq %r9
   	movq %rsp,%rdi
   	call irq_handler2
   	popq %r9
   	popq %r8
   	popq %rsi
   	popq %rbp
   	popq %rdx
   	popq %rcx
   	popq %rbx
   	popq %rax
   	popq %rdi
   	addq $16 ,%rsp
   	iretq

irq_common_stub:
   pushq %rax               
   pushq %rbx
   pushq %rcx
   pushq %rdx
   pushq %rsi
   pushq %rdi
   pushq %rbp
   pushq %r8
   pushq %r9
   pushq %r10
   pushq %r11
   pushq %r12
   pushq %r13
   pushq %r14
   pushq %r15
  // movq %ax, %ds              
  // pushq %eax               
  // movq %ax, $0x10 
  // movq %ds, %ax
  // movq %es, %ax
  // movq %fs, %ax
  // movq %gs, %ax

   call irq_handler

   //popq %eax
  
   popq	 %r15
   popq	 %r14
   popq	 %r13
   popq	 %r12
   popq	 %r11
   popq	 %r10
   popq	 %r9
   popq	 %r8
   popq  %rbp
   popq  %rdi
   popq  %rsi
   popq  %rdx
   popq  %rcx
   popq  %rbx
   popq  %rax
   
    
   //add esp, 8     
   //sti
   iretq    

irq_common_stub1:
   pushq %rax
   pushq %rbx
   pushq %rcx
   pushq %rdx
   pushq %rsi
   pushq %rdi
   pushq %rbp
   pushq %r8
   pushq %r9
   pushq %r10
   pushq %r11
   pushq %r12
   pushq %r13
   pushq %r14
   pushq %r15

    //movq %ax, %ds
    //pushq %eax
    //movq %ax, $0x10
    //movq %ds, %ax
    //movq %es, %ax
    //movq %fs, %ax
    //movq %gs, %ax
    call irq_handler1
    //popq %ebx 

   popq  %r15
   popq  %r14
   popq  %r13
   popq  %r12
   popq  %r11
   popq  %r10
   popq  %r9
   popq  %r8
   popq  %rbp
   popq  %rdi
   popq  %rsi
   popq  %rdx
   popq  %rcx
   popq  %rbx
   popq  %rax

    //add esp, 8
    //sti
   iretq 

