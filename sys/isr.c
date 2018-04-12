#include <sys/defs.h>
#include "isr.h"
#include<sys/syscall.h>
#include<sys/process.h>
#include "des_table.h"
#include "common.h"
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/page.h>

isr_t int_handlers[256];
int i=0;
extern struct pcb *current;
int capskey=0;
int controlkey=0;
int shiftkey=0;
extern char * video;
int scanFlag=0;
static volatile char buffer[512];
int index=0;
int localEcho=0;
extern int childFlag;

// keyboard key mapping referred from http://www.osdever.net/bkerndev/Docs/keyboard.htm

uint64_t scanfstdin(uint64_t address, int count){
	//kprintf("in stdin scan");
	scanFlag=1;
	localEcho=1;
	__asm__ __volatile__ ("sti");
	while (scanFlag!=0)
	{
		//kprintf("scanned input: %s",buffer);
		//__asm__ __volatile__ ("hlt");
		//kprintf("%d\n",scanFlag);
		printCursor();

	}
	//kprintf("Hello");
	//kprintf("%s",buffer);
	localEcho=0;
	buffer[index++]='\0';
	memcpy((void*)address,(void*)buffer,index);
	//kprintf("%s",address);
	//memset((void*)buffer,0,index);

	int scanLength=index;
	index=0;
	return scanLength;
}

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


void register_interrupt_handler(uint16_t n, isr_t handler){
//	kprintf("register_handler");
  int_handlers[n] = handler;
}

void isr_install(){
	//idt_gate(0,(uint64_t)timer_routine,0x08);
	outb(0x20, 0x11);
    	outb(0xA0, 0x11);
    	outb(0x21, 0x20);
    	outb(0xA1, 0x28);
    	outb(0x21, 0x04);
    	outb(0xA1, 0x02);
    	outb(0x21, 0x01);
    	outb(0xA1, 0x01);
    	outb(0x21, 0x0);
    	outb(0xA1, 0x0); 
	init_idt();
}

void isr_handler(registers_t* regs){
//	kprintf("received");
}

void irq_handler(registers_t regs){
//kprintf("hello\n");
  /* if (regs.int_no >= 40){
       outb(0xA0, 0x20);
   }*/
//   outb(0xA0, 0x20);
   outb(0x20, 0x20);

//   if (int_handlers[regs.int_no] != 0){
  {  // kprintf("in if");
	 isr_t handler = int_handlers[32];
       handler(regs);
   }
//outb(0x20,0x20);
}

void irq_handler2(registers_t *regs){
/*	uint64_t noret;
 	uint64_t retval;
        noret  = regs->rax;
        retval = syscall(noret,regs->rdi,regs->rsi,regs->rdx);
        regs->rax = retval;

	*((int64_t *)current->init_kern - 9) = retval;
	outb(0xA0,0x20);
	outb(0x20,0x20);
*/	if (regs->int_no == 0){
		kprintf("\nDivide by Zero!------Please rebooot");
		 while(1);
		//pageFaultHandler(regs);
	}

	else if (regs->int_no == 10){
		kprintf("\nInvalid TSS Exception!------Please rebooot");
		 while(1);
		//pageFaultHandler(regs);
	}
	else if (regs->int_no == 13){
		kprintf("\nGeneral Protection Fault!------Please rebooot");
		while(1);
		//pageFaultHandler(regs);
	}
	else if (regs->int_no == 14){
		pageFault(regs);
		//pageFaultHandler(regs);
	}
	else if (regs->int_no == 128){
		//int flag=0;
		uint64_t ret= syscall(regs->rax,regs->rbx,regs->rcx,regs->rdx);
		//kprintf("%din kernel",ret);
		regs->rax=ret;
		if(childFlag==1)
		{
			uint64_t *	temp =(uint64_t*)(current->kstack + 232);
			*temp = ret;
			childFlag=0;
		}
		//kprintf("%x",*((int64_t *)current->kstack - 9));	
		outb(0xA0,0x20);
	}
	outb(0x20,0x20);
}

void irq_handler1(registers_t* regs){
	i++;
	//kprintf("keyboard interrupt %d",i);
	//outb(0x20,0x20);
	//isr_t handler = int_handlers[33];
       	//handler(regs);
	//uint64_t p=0xB8000;
	char character=' ';
	//uint64_t p=0xB8EE0;
	//video=(char*)p;
    	//video=(char*)p;
	unsigned char scancode = inb(0x60);
	unsigned char value=kbdus[scancode];
	/*if (scancode == 0x2a)
	{
		value=value-32;
	}*/
	//kprintf("%c",value);
	if(scancode == 0x1d) //
	{
		//kprintf("^");
		controlkey=1;
	}
	else if(scancode == 0x9d)
	{
		controlkey=0;
	}
	else if(controlkey==1)
	{
		if((scancode & 0x80)!=0x80)
                {
                        //value=kbdus[scancode];
                        //kprintf("^%c",value-32);
                }

		//kprintf("^%c",kbdus[scancode]);
		//controlkey=0;
	}
	else if(scancode == 0x2a || scancode == 0x36)
		shiftkey=1;
	else if(scancode == 0xaa || scancode == 0xb6)
		shiftkey=0;
	else if (scancode == 0x3a){
		if(capskey==0)capskey=1;
		else if(capskey==1)capskey=0;
	}
	else if (capskey==1){
	if((scancode & 0x80)!=0x80)
                {
                        value=kbdus[scancode];
			kprintf("%c",value-32);	
		}
	}
	else if(shiftkey==1)
	{
	//kprintf("%c",kbdus[scancode]);
		if((scancode & 0x80)!=0x80)
		{
			value=kbdus[scancode];
			if(value == '1'){
				//kprintf("!");
				character='!';
			}
			else if(value=='2') {
				character='@';
				//kprintf("@");
			}
			 else if(value=='3') {
				character='#';
				//kprintf("#");
			}
			 else if(value=='4')
				character='$';
			 else if(value=='5')
				character='%';
			 else if(value=='6')
				character='^';
			 else if(value=='7')
				character='&';
			 else if(value=='8')
				character='*';
			 else if(value=='9')
				character='(';
			 else if(value=='0')
				character=')';
			 else if(value=='-')
				character='_';
			 else if(value=='=')
				character='+';
			 else if(value=='[')
				character='{';
			 else if(value==']')
				character='}';
			 else if(value=='\\')
				character='|';
			 else if(value==';')
				character=':';
			 else if(value=='\'')
				character='"';
			 else if(value==',')
				character='<';
			 else if(value=='.')
				character='>';
			 else if(value=='/')
				character='?';
			 else if(value=='`')
				character='~';
			else if(value>=97||value<=122)
			{
				if(capskey==0)
					character=value-32;
				else
					character=value;
				//character=kbdus[scancode];
			}
			buffer[index]=character;
				if(localEcho==1)
				kprintf("%c",buffer[index]);


		//kprintf("%s",buffer);
				index++;
		}
	}
	else if(scancode == 0x0e) // backspace
	{
		if(localEcho==1 &&index>0)
		{
			index--;
			backspace();
		}	
		
		//buffer[index]=" ";
		//kprintf("^H");
	}
	
	else if(scancode == 0x0F)
        {
          //      kprintf("^I");
        }
	else if(scancode == 0x01) //
        {
            //    kprintf("^[");
        }
	else if(scancode == 0x53) //delete
        {
              //  kprintf("^?");
        }
	else if(scancode == 0x1C) //enter
        {
				scanFlag=0;

                //kprintf("^M");
			//kprintf("%s",buffer);
        }
	else if(scancode == 0x48) //up arrow
        {
                //kprintf("^[[A");
        }
	 else if(scancode == 0x4B) //left arrow
        {
                //kprintf("^[[D");
        }
        else if(scancode == 0x50) //down arrow
        {
                //kprintf("^[[B");
        }
        else if(scancode == 0x4D) //right arrow
        {
                //kprintf("^[[C");
        }

	else if((scancode & 0x80) != 0x80 && shiftkey!=1)
	{
		//kprintf("%c",kbdus[scancode]);
		character=kbdus[scancode];
		buffer[index]=character;
		if(localEcho==1)
		kprintf("%c",buffer[index]);


		//kprintf("%s",buffer);
		index++;
	}
	
	//if((scancode & 0x80)!=0x80)
	//kprintf("    ");
	/*else if (scancode == ){

	}*/
	//kprintf("%c",kbdus[scancode]);


	outb(0x20,0x20);
}
