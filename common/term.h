#ifndef _TERM_H_
#define _TERM_H_

#include "../common/types.h"

enum term_color {
	TERM_COL_BLACK = 0,
	TERM_COL_BLUE = 1,
	TERM_COL_GREEN = 2,
	TERM_COL_CYAN = 3,
	TERM_COL_RED = 4,
	TERM_COL_MAGENTA = 5,
	TERM_COL_BROWN = 6,
	TERM_COL_LIGHT_GREY = 7,
	TERM_COL_DARK_GREY = 8,
	TERM_COL_LIGHT_BLUE = 9,
	TERM_COL_LIGHT_GREEN = 10,
	TERM_COL_LIGHT_CYAN = 11,
	TERM_COL_LIGHT_RED = 12,
	TERM_COL_LIGHT_MAGENTA = 13,
	TERM_COL_YELLOW = 14,
	TERM_COL_WHITE = 15,
};


typedef struct color_st {
	uint8_t fg : 4;  // foreground color
	uint8_t bg : 4;  // background color
} color_t;

extern void term_init();
extern void term_clear();

extern uint8_t term_getcolors();
extern void term_setcolors(uint8_t col);
extern void term_setfgcolor(enum term_color foreground);
extern void term_setbgcolor(enum term_color background);

extern void term_setchar(char c, int x, int y, enum term_color fg, enum term_color bg);
extern void term_putc(char c);
extern void term_puts(char *s);
extern void term_printf(char *fmt, ...);
extern void term_getcursor(int *x, int *y);
extern void term_setcursor(int x, int y);

#endif
