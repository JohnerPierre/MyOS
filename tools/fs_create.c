#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void format(){
	printf("./fs_create <fsname> <label> <blockSize> <multiBlock> \n");
	return;
}

int main(int argc, char *argv[]){
	if(argc > 4){
		int blockSize = atoi(argv[3]);
		int multiBlock = atoi(argv[4]);
		
		if(blockSize % 512 == 0){
			create(argv[1], argv[2], blockSize, multiBlock);
		}else{
			printf("BlockSize should be a multiple of 512 \n");
		}
	}else{
		format();
	}
	return 0;
}
