/*
 Main Library for user application
*/

#include "ulibc.h"

/*
 Open File with File Name
*/
int file_open(char* filename){
    return syscall(SYSCALL_FILE_OPEN, (uint32_t)filename, 0, 0, 0);  
}

/*
 Create File iterator
*/
file_iterator_t file_iterator(){
    file_iterator_t *it = NULL;
	syscall(SYSCALL_FILE_ITERATOR, (uint32_t)it, 0, 0, 0);
	return (file_iterator_t)*it;
}

/*
 if iterator has next
*/
bool file_has_next(file_iterator_t *it){
    bool* result = NULL;
	syscall(SYSCALL_FILE_HAS_NEXT, (uint32_t)it, (uint32_t)result, 0, 0);
	return (bool)*result;
}

/*
 Mive iterator to the next file
*/
void file_next(char *filename, file_iterator_t *it){
	syscall(SYSCALL_FILE_NEXT, (uint32_t)filename, (uint32_t)it, 0, 0);
    return;
}

/*
 Read File with descriptor file number
*/
int file_read(int fd, void *buf, uint count){
    return  syscall(SYSCALL_FILE_READ, (uint32_t)fd, (uint32_t)buf, (uint32_t)count, 0);
}

/*
 Stat of a File
*/
int file_stat(char *filename, stat_t *stat){
    return syscall(SYSCALL_FILE_STAT, (uint32_t)filename, (uint32_t)stat, 0, 0);
}

/*
 Remove File by Name
*/
int file_remove(char *filename){
    return syscall(SYSCALL_FILE_REMOVE, (uint32_t)filename, 0, 0, 0);
}

/*
 Close File
*/
void file_close(int fd){
    syscall(SYSCALL_FILE_CLOSE, (uint32_t)fd, 0, 0, 0);
    return;
}

/*
 Get a char input by user
*/
char getc(){
    char* result = NULL;
    syscall(SYSCALL_GETC, (uint32_t)result, 0, 0, 0);
    return (char)*result;
}

/*
 Display char
*/
void putc(char c){
	syscall(SYSCALL_PUTC, (uint32_t)c, 0, 0, 0);
    return;
}

/*
 Display String
*/
void puts(char *s){
    syscall(SYSCALL_PUTS, (uint32_t)s, 0, 0, 0);
	return;
}

/*
* Print in screen 
* char* s string to display
* ... number of parameter variable accept %s,%x,%c,%d
* % alone is not suported & and number arguments must be equals to % in string
*/
void printf(char *s, ...){
    int *ap;
    int i = 0;
    char data[10];
    
    ap = (int*)&s;

    while(s[i] != '\0'){
        if(s[i] == '%'){
            //Next Argument
            ap++;
            if(s[i+1] == 'd'){
                itos((int)*ap, data, 10);
                puts(data);
            }
			else if(s[i+1] == 'x'){
                itos((int)*ap, data, 16);
                puts(data);
            }
            else if(s[i+1] == 'c'){
                putc((char)*ap);
            }
			else if(s[i+1] == 's'){ 
                puts((char*)*ap);
            }
			i++;
        }else{
			putc(s[i]);
		}
		i++;
    }
    return;
}

/*
 Passive Sleep
*/
void sleep(uint ms){
	syscall(SYSCALL_SLEEP, (uint32_t)ms, 0, 0, 0);
    return;
}

/*
 Read n char until Enter press
*/
void read_string(char* buffer){
    char c = getc();
    int cnt = 0;
    while(c != 13){
        buffer[cnt]= c;
        cnt++;
        putc(c);
        c = getc();
    }
    buffer[cnt]= 0;
    return;
}

/*
 Delet Space from File
*/
char* trim(char* buffer){
  int i = 0;
  int j = strlen ( buffer ) - 1;
  int k = 0;
 
  while ( buffer[i] == ' ' && buffer[i] != '\0' )
    i++;
 
  while ( buffer[j]  == ' ' && j >= 0 )
    j--;
 
  while ( i <= j )
    buffer[k++] = buffer[i++];
 
  buffer[k] = '\0';
 
  return buffer;  
}

/*
 Char to uint convertor
*/
uint atoi(char* value){
    uint res = 0;
  
    for (int i = 0; value[i] != '\0'; ++i)
        res = res*10 + value[i] - '0';
  
    return res;
}

/*
* Convert Int to String
* Integer value number to convert
* Char* result  contain the result
* Integer b base of convert
*/
void itos(int value, char* result, int b){
    char digit[] = "0123456789ABCDEF";
    int count = 0;
    int d = value;
    int i;
    while(d != 0){
        d = d / b;
        count++;
    }
	if(count == 0)
		count++;
    result[count] = '\0';
    d = value;
    for(i=count-1;i >= 0; i--){
        result[i] = digit[d % b];
        d = d / b;
    }
    return;
}

/*
 Compare text lenght to line lenght
*/
bool starts_with(char* text, char* line){
	int lenpre = strlen(text),
           lenstr = strlen(line);
    return lenstr < lenpre ? false : strncmp(text, line, lenpre) == 0;
}

/*
 String compare in n char
*/
int strncmp(const char *s1, const char *s2, int n){
	for ( ; n > 0; s1++, s2++, --n)
		if (*s1 != *s2)
			return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
		else if (*s1 == '\0')
			return 0;
    return 0;
}

/*
 String Compare
*/
int strcmp(const char *s1, const char *s2){
    for ( ; *s1 == *s2; s1++, s2++)
	if (*s1 == '\0')
	    return 0;
    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}

/*
 Lenght of String
*/
int strlen(char* p){
	int i;
    for (i = 0; p[i] != '\0'; i++) ;
    return i;
}

/*
 Execute a file
*/
int exec(char* filename){
    return syscall(SYSCALL_EXEC, (uint32_t)filename, 0, 0, 0);
}
