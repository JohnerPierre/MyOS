#include "ulibc.h"

void list_files() {
	file_iterator_t it = file_iterator();

	while (file_has_next(&it)) {
		char name[256];
		file_next(name, &it);
		stat_t stat;
		file_stat(name, &stat);
		printf("%s (%d)\n", name, stat.size);
	}
}

void main(){
    
    puts("Welcome to Test Script For Ulibc\n");
    
    puts("File Section\n");
    
    puts("Iterator , list Files\n");
	list_files();
	
	sleep(2000);
    
    puts("Read a , read , close  File\n");
    int fd = file_open("artscii.txt");
    if (fd == -1) {
		puts("Failed Open artscii.txt \n");
	}
	
	sleep(2000);
	
	int sz = 256;
	char buf[sz+1];
	int n;
	do {
		n = file_read(fd, buf, sz);
		buf[n] = 0;
		puts(buf);
		
	} while (n);
	file_close(fd);
	
	sleep(2000);
    
    if (file_remove("artscii.txt") == -1)
		puts("rm: failed deleting artscii.txt\n");
	
	puts("List without artscii\n");
	list_files();
	
	sleep(2000);
	
	puts("Testing putc\n");
	putc('a');
	putc('b');
	putc('c');
	putc('d');
	puts("\nSame as\n");
	puts("abcd\n");
	sleep(2000);
	
	puts("Testing Printf\n");
	printf("%d %d %s %c ", 12, 20, "Hello", 'G');
    puts("\nSame as\n");
	puts("12 20 Hello G\n");
	sleep(2000);
	
	puts("Testing Exec\n Start Game...");
	if (exec("game") == -1) {
				puts("Failed executing game\n");
	}
	puts("Welcom Back to Testing\n");
	puts("test exit() in while(true) loop");
	while(1){
	    exit();
	}
    
}