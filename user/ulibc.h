#ifndef _ULIBC_H_
#define _ULIBC_H_

#include "../common/types.h"
#include "../common/syscall_nb.h"

typedef struct{
	uint32_t nextFile;
} file_iterator_t;

typedef struct{
	char name[51];
	uint32_t size;
} stat_t;

int file_open(char* filename);
file_iterator_t file_iterator();
bool file_has_next(file_iterator_t *it);
void file_next(char *filename, file_iterator_t *it);
int file_read(int fd, void *buf, uint count);
int file_stat(char *filename, stat_t *stat);
int file_remove(char *filename);
void file_close(int fd);


char getc();
void putc(char c);
void puts(char *s);
void printf(char *s, ...);
void sleep(uint ms);
void read_string(char* buffer);
char* trim(char* buffer);
uint atoi(char* value);
bool starts_with(char* text, char* line);
void itos(int value, char* result, int b);

int strncmp(const char *s1, const char *s2, int n);
int strcmp(const char *s1, const char *s2);
int strlen(char* p);
int exec(char* filename);

extern void exit();
extern int syscall(uint32_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

#endif
