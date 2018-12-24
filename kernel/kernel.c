#include "multiboot.h"
#include "gdt.h"
#include "library.h"
#include "pic.h"
#include "idt.h"
#include "timer.h"
#include "x86.h"
#include "keyboard.h"
#include "screen.h"
#include "fs.h"
#include "task.h"
#include "../common/term.h"
#include "test1.c"
#include "test2.c"
#define BuffSize 600

int kernel (multiboot_info_t* info){
	term_init();
	
    //Display Ram
	term_printf("Display initialization\n"); 
	//Init
	term_printf("Memory : %d\n",info->mem_upper);
	//Init Gdt
	term_printf("GDT initialization\n");
    // init table descripteur globaux	
	gdt_init();
	term_printf("PIC initialization\n");
	pic_init();
	term_printf("IDT initialization\n");
	idt_init();
	term_printf("Timer initialization\n");
	timer_init(100);
	term_printf("Keyboard initialization\n");
	keyboard_init();
	term_printf("Enable interrupt\n");
	sti();
	term_printf("Descriptor  initialization\n");
	initDescriptor();
	
    #ifdef debug
        test2();
    #else
	
    #endif
	
	int fd = file_open("artscii.txt");
	char buf[BuffSize+1];
	buf[BuffSize] = 0;
	
	while(file_read(fd,buf,BuffSize) != 0){
	    sleep(100);
		term_printf("%s", buf);
		memset(&buf,0,BuffSize);
	}
	
	term_printf("\n");
	run_task("shell");
	halt();

    return 0;
}