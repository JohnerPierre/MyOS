#include "screen.h"

//default value black background with white text
uint8_t background_color = 0;
uint8_t text_color = 15;
static const int screen = 0xB8000;

/*
* Initialization of the screen
*/
void initDisplay(){
	setBackgroundText(0);
	setColorText(15);
    cleanDisplay();
    setCursor(0);
    return;
}

/*
* Set all black the screen
*/
void cleanDisplay(){
	int value = ((0 << 4) | 15) << 8 | (int)' ';
	int i = 0;
    
	for(i=0;i<=4000;i=i+2){
		uint16_t* port = (uint16_t*)(i+screen);
		*port = value;
	}
    return;
}

/*
* Get color
*/
uint8_t getColor(){
    return (getBackgroundText() << 4) | getColorText();
}

/*
* Set Color of the Text
* uint8_t data value of the color in 8bits
*/
void setColorText(uint8_t data){
    text_color = data;
    return;
}

/*
* Get Color of the Text
* return uint8_t  value of the color in 8bits
*/
uint8_t getColorText(){
    return text_color;
}

/*
* Set Color of the BackGround
* uint8_t data value of the color in 8bits
*/
void setBackgroundText(uint8_t data){
    background_color = data;
    return;
}

/*
* Get Color of the BackGround
* return uint8_t  value of the color in 8bits
*/
uint8_t getBackgroundText(){
    return background_color;
}

/*
* Display char in the position of the cursor
* char c char to display
*/
void displayCharCursor(char c){
    if((uint8_t)c != 0){ 
        uint16_t pos = getCursor();
        uint16_t* port = (uint16_t*)(screen+2*pos);
        uint16_t value = getColor() << 8 | (uint8_t)c;
        *port =  value;
        pos++;
        setCursor(pos);
    }
    return;
}

/*
* Display char in the position of the cursor
* char* s strinf to display
*/
void displayStringCursor(char* s){
    int i = 0;
	while(s[i] != '\0'){
        displayCharCursor(s[i]);
        i++;
    }
    return;
}