#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void format(){
	printf("./fs_del <filename> <fsname> \n");
	return;
}

int main(int argc, char *argv[]){
	if(argc > 2){
		if( access( argv[2], F_OK ) != -1 ) 
			delete(argv[1], argv[2]);
		else
			printf("File doesn't exist ! \n");
	}else{
		format();
	}
	return 0;
}
