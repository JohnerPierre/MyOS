#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void format(){
	printf("./fs_list <fsname> \n");
	return;
}

int main(int argc, char *argv[]){
	if(argc > 1){
		if( access( argv[1], F_OK ) != -1 ) 
			list(argv[1]);
		else
			printf("File doesn't exist ! \n");
	}else{
		format();
	}
	return 0;
}
