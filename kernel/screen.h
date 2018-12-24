#include "../common/types.h"
#include "cursor.h"

void initDisplay();

void cleanDisplay();

uint8_t getColor();

void setColorText(uint8_t data);

uint8_t getColorText();

void setBackgroundText(uint8_t data);

uint8_t getBackgroundText();

void displayCharCursor(char c);

void displayStringCursor(char* s);