#include "../common/types.h"
#include "port.h"

void setCursor(uint16_t value);

uint16_t getCursor();

void setCursorPos(uint8_t x, uint8_t y);

void getCursorPos(uint8_t* x, uint8_t* y);