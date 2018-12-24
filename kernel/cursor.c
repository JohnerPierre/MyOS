#include "cursor.h"

static const int LINE = 25;
static const int COLUMN = 80;

/*
* Set Position of the cursor
* uint16_t value value of the cursor in 16bits
*/
void setCursor(uint16_t value){
    uint8_t MSB = (value >> 8) & 0xFF;
    uint8_t LSB = value & 0xFF;

    outb(0x3d4,0XE);
    outb(0x3d5,MSB);

    outb(0x3d4,0XF);
    outb(0x3d5,LSB);
    
    return;
}

/*
* Get Position of the cursor
* return uint16_t  value of the cursor in 16bits
*/
uint16_t getCursor(){
    int MSB,LSB = 0;   

    outb(0x3d4,0XE);
    MSB = inb(0x3d5);

    outb(0x3d4,0XF);
    LSB = inb(0x3d5);
    
    int value = (MSB<<8)+LSB;
    return value;
}


/*
* Set Position of the cursor
* uint8_t x x pos of the cursor in 8bits
* uint8_t y y pos of the cursor in 8bits
*/
void setCursorPos(uint8_t x, uint8_t y){
    setCursor(COLUMN * y + x);
    return;
}

/*
* Get Position of the cursor
* uint8_t x x pos of the cursor in 8bits
* uint8_t y y pos of the cursor in 8bits
*/
void getCursorPos(uint8_t* x, uint8_t* y){
    uint16_t value = getCursor();
    *x = value % COLUMN;
    *y = value / COLUMN;
    return;
}