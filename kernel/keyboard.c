#include "keyboard.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATE_PORT 0x64
#define BUFFER_SIZE 128

uint8_t keyboardMap[128] =
{
     0, /* Esc*/
     27, /* ret */
     49, 50, 51, 52, 53, 54, 55, 56, 57, 48,/*  0-9 */
     39,94,/* ' ^ */
     8,/* BackSpace */
     9,/* Tab */	
     113,119,101,114,116,122,117,105,111,112,/* q-p */
     138,34, /* è ¨ */
     13,/* Enter */
     0,/* Caps */
     97,115,100,102,103,104,106,107,108, /* a-l */
     130,133,21, /* é à § */
     0,	/* Left Shift */
     36, /* $ */
	 121,120,99,118,98,110,109, /* y-m */
	 44,46,45,   /* , . - */
     0,	/* Right Shift */
     42,  /* * */
     0,	/* Alt */
     32,	/* Space bar */
     0,	/* Alt Gr */
     0, 0,   0,   0,   0,   0,   0,   0,   0, 0,	/*  F1- F10 */
     0,	/* Num lock*/
     0,	/* Scroll Lock */
     0,	/* Home key */
     0,	/* Up Arrow */
     0,	/* Page Up */
     45,/*- */
     0,	/* Left Arrow */
     0,/* pad num 5 */
     0,	/* Right Arrow */
     43,/* + */
     0,	/* End key*/
     0,	/* Down Arrow */
     0,	/* Page Down */
     0,	/* Insert Key */
     0,	/* Delete Key */
     251,   
	 252,
	 60,/* <  */
     0,	/* F11 Key */
     0,	/* F12 Key */
     0	/*  other undefined */
    
};

uint8_t keyboardMapShift[128] =
{
     0, /* Esc*/
     27, /* ret */
     43, /* + */
     34, /* " */
     42, /* * */
     135, /* ç */
     37, /* % */
     38, /* & */
     47, /* / */
     40, /* ( */
     41, /* ) */
     61, /* = */
     63,96,/* ? `*/
     8,/* BackSpace */
     9,/* Tab */
     81,87,69,82,84,90,85,73,79,80,/* Q-P */
     129,33, /* ü ! */
     13,/* Enter */
     0,/* Caps */
     65,83,68,70,71,72,74,75,76, /* A-L */
     148,132,248, /* ö ä ° */
     0,	/* Left Shift */
     156, /* £ */
	 89,88,67,86,66,78,77, /* Y-M */
	 59,58,95, /* ; : _ */
     0,	/* Right Shift */
     42,  /* * */
     0,	/* Alt */
     32,	/* Space bar */
     0,	/* Alt Gr */
     0, 0,   0,   0,   0,   0,   0,   0,   0, 0,	/*  F1- F10 */
     0,	/* Num lock*/
     0,	/* Scroll Lock */
     0,	/* Home key */
     0,	/* Up Arrow */
     0,	/* Page Up */
     45,/*- */
     0,	/* Left Arrow */
     0,/* pad num 5 */
     0,	/* Right Arrow */
     43,/* + */
     0,	/* End key*/
     0,	/* Down Arrow */
     0,	/* Page Down */
     0,	/* Insert Key */
     0,	/* Delete Key */
     251,   
	 252,
	 62,/* > */
     0,	/* F11 Key */
     0,	/* F12 Key */
     0	/*  other undefined */
    
};

int buffer[BUFFER_SIZE];
int current_buffer = 0;
int flag_shift = 0;
int multiPres = -1;

void keyboard_init() {

}

int isShift(int x){
    int flag = 0;
    // Left-Right Shift in press & depress
    if(x==42 || x==54 || x==170 || x==184){
        flag = 1;
    }
    return flag;
}

void keyboard_handler() {
    int status = inb(KEYBOARD_STATE_PORT);
    if(current_buffer<BUFFER_SIZE){
        if ((status & 0x01)) {
            int v = inb(KEYBOARD_DATA_PORT);
        	 if(v & (1 << 7)){
        	    if(isShift(v)){
        	       flag_shift = 0;
        	    }
        	 }
        	 else // pression
        	 {
        	   if(isShift(v)){
        	       flag_shift = 1;
        	   }
        	   else{
        	         buffer[current_buffer] = v; 
	                 current_buffer=current_buffer+1;
        	   }
        	 }
	      }
    }
    else
    {
        uint8_t old = getColorText();
        setColorText(12);
        print("Buffer Full");
        setColorText(old);
    }

}

uint8_t getc() {
    while(current_buffer==0){}
    
    current_buffer = current_buffer-1;
    if(flag_shift)
        return keyboardMapShift[buffer[current_buffer]];
    else
        return keyboardMap[buffer[current_buffer]];
}

// Non-blocking call. Return 1 if a key is pressed
int keypressed() {
    int v = inb(KEYBOARD_DATA_PORT);
    if(v & (1 << 7)){
        return 0;
    }
    return 1;
}

