#include "isr.h"
#include "common.h"
#include "timer.h"
#include <sys/defs.h>           
#include <sys/kprintf.h>
#include <sys/process.h>

uint64_t count=0;
extern struct pcb *current;
int sec=0;
int kk=0;
int min=0;
int hour=0;
uint64_t freq=100;
uint32_t def_freq=1193180;
uint64_t time=0,temp=0;
void time_func()
{
	count++;
	int x=0,y=0;
	uint64_t p=0xFFFFFFFF800B8000;
	extern char* video,xp,yp;
	x=xp;
	y=yp;
	char* tempV=video;
	video=(char*)p;
//kprintf("count %d",count);
//kprintf("abc \n");
//krpintf("");
//kprintf("00:00:00");
if (count%90==0)
{
	time++;
	temp=time;
	sec=temp%60;
	temp=temp/60;
	min=temp%60;
	temp=temp/60;
	hour=hour%60;
	if(hour<10)
	{
		kprintf("0");
		kprintf("%d:",hour);
	}
	else
		kprintf("%d:",hour);
	if(min<10)
        {
		kprintf("0");
                kprintf("%d:",min);
        }
        else
                kprintf("%d:",min);
	if(sec<10)
        {
                kprintf("0");
                kprintf("%d",sec);
        }
        else
                kprintf("%d",sec);
	if(current->sleep>0) {
		//kprintf("%d\n",current->sleep);
		current->sleep--;
	}
	/*p=0xB8200;
	video=(char*)p;
	kprintf("%d",time);*/
}
video=tempV;
	xp=x;
	yp=y;
//kprintf("%d\n",sec);
}
void key_press(){
//unsigned char scancode=
kprintf("key pressed %d",kk);
}
void init_timer(uint32_t freq){
	//kprintf("timer123");
//	kprintf("00:00:00");
	__asm__ __volatile__ ("sti");	
	register_interrupt_handler(32, time_func);
	
	uint32_t d=def_freq / freq;
	outb(0x43,0x36);
	outb(0x40,(uint8_t)(d & 0xFF));
	outb(0x40,(uint8_t)((d>>8)&0xFF));
}
void key(){
//kprintf("keyboard");
__asm__ __volatile__ ("sti");
        //register_interrupt_handler(33, key_press);
}

