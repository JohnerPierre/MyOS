#include "timer.h"
#define canal0 0x40
#define command 0x43

const int PIT = 1193180;
uint tick = 0;
uint32_t current_freq = 0;

void timer_init(uint32_t freq_hz) {
	tick = 0;
	current_freq = freq_hz;
	uint16_t d = PIT / freq_hz;
	//write command 
	outb(command, 0x36);
	//update canal 0 with diviseur
	outb(canal0, d & 0xff);
	//update canal 0 with diviseur
	outb(canal0, d >> 8);
}

void timer_handler() {
	//uint8_t x,y;
	tick++;
	/*
	getCursorPos(&x, &y);
	setCursorPos(0,24);
	print("Tick: %d", tick);
	setCursorPos(x, y);*/
}

uint get_ticks() {
	return tick;
}

void sleep(uint ms) { 
	uint time = current_freq*ms/1000+tick;
	while(tick <= time){}
}


