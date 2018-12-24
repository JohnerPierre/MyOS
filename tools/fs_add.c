#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void format(){
	printf("./fs_add <filename> <fsname> \n");
	return;
}

int main(int argc, char *argv[]){
	if(argc > 2){
		if( access( argv[1], F_OK ) != -1 ){
			if( access( argv[2], F_OK ) != -1 ) 
				add(argv[1], argv[2]);
			else
				printf("File 2 doesn't exist ! \n");
		}
		else{
			printf("File 1 doesn't exist ! \n");
		}
	}else{
		format();
	}
	return 0;
}
