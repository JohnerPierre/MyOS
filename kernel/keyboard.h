#include "port.h"
#include "library.h"

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

extern void keyboard_init();
extern void keyboard_handler();
extern uint8_t getc();
extern int keypressed();


#endif
