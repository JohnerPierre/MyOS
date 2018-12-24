#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//one because superblock can't be more than 512 octet
#define nbrBlockForSuperBlock 1

/*
* Create super block
* char* label name of fs
* uint32_t blockSize size of a block
* uint32_t multiBlock number of block
* uint32_t availableBlocksSize number of available block
* uint32_t fileBlocksSize number of file block
* uint32_t RefBlocksSize number of ref block
*/
SuperBlock* createSuperBlock(char* label, uint32_t blockSize, uint32_t multiBlock, uint32_t availableBlocksSize, uint32_t fileBlocksSize, uint32_t RefBlocksSize){
    SuperBlock* superBlock = malloc(sizeof(SuperBlock));
    strncpy(superBlock->label,label,10);
    superBlock->version = 1;
    superBlock->type = 0;
    superBlock->blockSize = blockSize;
    superBlock->multiBlock = multiBlock;
    superBlock->availableBlocksSize = availableBlocksSize;
    superBlock->fileBlocksSize = fileBlocksSize;
    superBlock->refBlocksSize = RefBlocksSize;
    return superBlock;
}

/*
* Get size of Available array
* SuperBlock* SuperBlock information of the superBlock
*/
uint getNumberAvailable(SuperBlock* superBlock){
	return superBlock->availableBlocksSize*superBlock->blockSize/sizeof(AvailableBlock);
}

/*
* Get size of File array
* SuperBlock* SuperBlock information of the superBlock
*/
uint getNumberFile(SuperBlock* superBlock){
	return superBlock->fileBlocksSize*superBlock->blockSize/sizeof(FileBlock);
}

/*
* Get size of Ref array
* SuperBlock* SuperBlock information of the superBlock
*/
uint getNumberRef(SuperBlock* superBlock){
	return superBlock->refBlocksSize*superBlock->blockSize/sizeof(RefBlock);
}

/*
* Get the superblock from fs
* FILE *fp fs
*/
SuperBlock* getSuperBlock(FILE *fp){
    fseek(fp, 0, SEEK_SET);
    SuperBlock* superBlock = malloc(sizeof(SuperBlock));
    fread(superBlock, sizeof(SuperBlock), nbrBlockForSuperBlock, fp);
    return superBlock;
}

/*
* Get the available block from fs
* FILE *fp fs
* SuperBlock* SuperBlock information of the superBlock
*/
AvailableBlock* getAvailableBlock(FILE *fp, SuperBlock* superBlock){
    fseek(fp, nbrBlockForSuperBlock*superBlock->blockSize, SEEK_SET); 
    AvailableBlock* availableBlock = malloc(sizeof(AvailableBlock) * getNumberAvailable(superBlock));
    fread(availableBlock, sizeof(AvailableBlock), getNumberAvailable(superBlock), fp);
    return availableBlock;
}

/*
* Get the file block from fs
* FILE *fp fs
* SuperBlock* SuperBlock information of the superBlock
*/
FileBlock* getFileBlock(FILE *fp, SuperBlock* superBlock){
    fseek(fp, (nbrBlockForSuperBlock+superBlock->availableBlocksSize)*superBlock->blockSize, SEEK_SET); 
    FileBlock* fileBlock = malloc(sizeof(FileBlock) * getNumberFile(superBlock));
    fread(fileBlock, sizeof(FileBlock), getNumberFile(superBlock), fp);
    return fileBlock;
}

/*
* Get the ref block in fs
* FILE *fp fs
* SuperBlock* SuperBlock information of the superBlock
*/
RefBlock* getRefBlock(FILE *fp, SuperBlock* superBlock){
    fseek(fp, (nbrBlockForSuperBlock+superBlock->availableBlocksSize+superBlock->fileBlocksSize)*superBlock->blockSize, SEEK_SET); 
    RefBlock* refBlock = malloc(sizeof(RefBlock) * getNumberRef(superBlock));
    fread(refBlock, sizeof(RefBlock), getNumberRef(superBlock), fp);
    return refBlock;
}

/*
* Set the available in fs
* FILE *fp fs
* SuperBlock* SuperBlock information of the superBlock
* AvailableBlock* availableBlock array to set
*/
void setAvailableBlock(FILE *fp, SuperBlock* superBlock, AvailableBlock* availableBlock){
    fseek(fp, (nbrBlockForSuperBlock)*superBlock->blockSize, SEEK_SET);
    fwrite(availableBlock, sizeof(AvailableBlock), getNumberAvailable(superBlock), fp);
    return;
}

/*
* Set the file in fs
* FILE *fp fs
* SuperBlock* SuperBlock information of the superBlock
* FileBlock* fileBlock array to set
*/
void setFileBlock(FILE *fp, SuperBlock* superBlock, FileBlock* fileBlock){
    fseek(fp, (nbrBlockForSuperBlock+superBlock->availableBlocksSize)*superBlock->blockSize, SEEK_SET);
    fwrite(fileBlock, sizeof(FileBlock), getNumberFile(superBlock), fp);
    return;
}

/*
* Set the ref in fs
* FILE *fp fs
* SuperBlock* SuperBlock information of the superBlock
* RefBlock* refBlock array to set
*/
void setRefBlock(FILE *fp, SuperBlock* superBlock, RefBlock* refBlock){
    fseek(fp, (nbrBlockForSuperBlock+superBlock->availableBlocksSize+superBlock->fileBlocksSize)*superBlock->blockSize, SEEK_SET);
    fwrite(refBlock, sizeof(RefBlock), getNumberRef(superBlock), fp);
    return;
}

/*
* Set the data in fs
* FILE *fp fs
* SuperBlock* SuperBlock information of the superBlock
* RefBlock* dataBlock array to set
* uint idBlock the number of the block to set
* uint size the size of the block
*/
void setDataBlock(FILE *fp, SuperBlock* superBlock, DataBlock* dataBlock, uint idBlock, uint size){
	fseek(fp, (nbrBlockForSuperBlock+superBlock->availableBlocksSize+superBlock->fileBlocksSize+superBlock->refBlocksSize+idBlock)*superBlock->blockSize, SEEK_SET);
    fwrite(dataBlock, sizeof(DataBlock), size, fp);
	if(size != superBlock->blockSize){
			DataBlock* padding = calloc(superBlock->blockSize - size, sizeof(DataBlock));
			fwrite(padding, sizeof(DataBlock), superBlock->blockSize-size,fp);
	}
    return;
}

/*
* Search next available block
* SuperBlock* SuperBlock information of the superBlock
* AvailableBlock* availableBlocks array to search
*/
int nextAvailableBlock(AvailableBlock* availableBlocks, SuperBlock* superBlock){
    uint i;
    for(i=0;i<getNumberAvailable(superBlock);i++){
        if(availableBlocks[i].available == 0)
            return i;
    }   
    return -1;
}

/*
* Search next available block
* SuperBlock* SuperBlock information of the superBlock
* fileBlock* fileBlock array to search
*/
int nextFileBlock(FileBlock* fileBlock, SuperBlock* superBlock){
    uint i;
    for(i=0;i<getNumberFile(superBlock);i++){
        if(fileBlock[i].available == 0)
            return i;
    }   
    return -1;
}

/*
* Search next available block
* SuperBlock* SuperBlock information of the superBlock
* RefBlock* refBlock array to search
*/
int nextRefBlock(RefBlock* refBlock, SuperBlock* superBlock){
    uint i;
    for(i=0;i<getNumberRef(superBlock);i++){
        if(refBlock[i].available == 0)
            return i;
    }   
    return -1;
}

/*
* Search file by name
* SuperBlock* SuperBlock information of the superBlock
* Char* fileName name of file to find
* FileBlock* fileBlock array to search
*/
int searchFile(FileBlock* fileBlock, char* fileName, SuperBlock* superBlock){
	uint i;
	for(i=0;i<getNumberFile(superBlock);i++){
		if(strcmp(fileBlock[i].name, fileName) == 0 && fileBlock[i].available == 1)
			return i;
	}
	return -1;
}

/*
* Create a new fs
* char* fileName name of the fs
* char* label of the fs
* uint32_t blockSize size of block
* uint32_t multiBlock number of block
*/
void create(char* fileName, char* label, uint32_t blockSize, uint32_t multiBlock){
    FILE *fp;
    fp = fopen(fileName, "w+");
    
    uint32_t availableBlocksSize = (multiBlock+blockSize-1) / blockSize;
    uint32_t fileBlocksSize = sizeof(FileBlock) * availableBlocksSize;
    uint32_t RefBlocksSize = sizeof(RefBlock) * availableBlocksSize;
    
    SuperBlock *superBlock = createSuperBlock(label, blockSize, multiBlock, availableBlocksSize, fileBlocksSize, RefBlocksSize);
    DataBlock* padding = calloc(blockSize-sizeof(SuperBlock), sizeof(DataBlock));
    AvailableBlock* availableBlock = calloc(getNumberAvailable(superBlock), sizeof(AvailableBlock));
    FileBlock* fileBlock = calloc(getNumberFile(superBlock), sizeof(FileBlock));
    RefBlock* refBlock = calloc(getNumberRef(superBlock), sizeof(RefBlock));
    DataBlock* dataBlock = calloc(multiBlock * blockSize, sizeof(DataBlock));
    
    fwrite(superBlock, sizeof(SuperBlock), nbrBlockForSuperBlock,fp);
    fwrite(padding, sizeof(DataBlock), blockSize-sizeof(SuperBlock),fp);
    fwrite(availableBlock, sizeof(AvailableBlock), getNumberAvailable(superBlock),fp);
    fwrite(fileBlock, sizeof(FileBlock), getNumberFile(superBlock),fp);
    fwrite(refBlock, sizeof(RefBlock), getNumberRef(superBlock),fp);
    fwrite(dataBlock, sizeof(DataBlock), multiBlock * blockSize,fp);
    
	free(superBlock);
	free(padding);
	free(availableBlock);
	free(fileBlock);
	free(refBlock);
	free(dataBlock);
	fclose(fp);
	
    return;
}

/*
* Add a file
* char* fileName name of the file
* char* label name of the fs file
*/
void add(char* fileName, char* label){
    FILE *fp ,*file;
    fp = fopen(label, "r+");
    file = fopen(fileName, "r");
    
    SuperBlock* superBlock = getSuperBlock(fp);
	AvailableBlock* availableBlock = getAvailableBlock(fp, superBlock);
    FileBlock* fileBlock = getFileBlock(fp, superBlock);
	RefBlock* refBlock = getRefBlock(fp, superBlock);
    
	//Create the file + find the first ref block
    int idFile = nextFileBlock(fileBlock, superBlock);
	int idRef = nextRefBlock(refBlock, superBlock);
	int idNew = idRef;
	int size = 0;	
	
	fileBlock[idFile].available = 1;
    strncpy(fileBlock[idFile].name,fileName,55);
	fileBlock[idFile].nbrBlock = 0;
	fileBlock[idFile].size = 0;
	fileBlock[idFile].firstBlock = idRef;
    
	DataBlock* dataBlock = calloc(superBlock->blockSize, sizeof(DataBlock));
	//Set the reference on block + if file is bigger add new ref
	while ((size = fread(dataBlock, 1, superBlock->blockSize, file)) != 0) {
		int idBlock = nextAvailableBlock(availableBlock, superBlock);
		if(idBlock >= 0){
			refBlock[idNew].available = 1;
			availableBlock[idBlock].available = 1;
			
			refBlock[idNew].dataBlock = idBlock;
			refBlock[idRef].refBlock = idNew;
			setDataBlock(fp, superBlock, dataBlock, idBlock, size);
			
			idRef = idNew;
			idNew = nextRefBlock(refBlock, superBlock);
			fileBlock[idFile].nbrBlock++;
			fileBlock[idFile].size += size;
		}
		else{
			printf("Not enought space for allocate next block \n");
		}
	}
	
	//rewrite changement
	setAvailableBlock(fp, superBlock, availableBlock);
    setFileBlock(fp, superBlock, fileBlock);
	setRefBlock(fp, superBlock, refBlock);
	
	free(superBlock);
	free(availableBlock);
	free(fileBlock);
	free(dataBlock);
	free(refBlock);
	fclose(fp);
	fclose(file);
        
    return;
}

/*
* Delete a file
* char* fileName name of the file
* char* label name of the fs file
*/
void delete(char* fileName, char* label){
	FILE *fp ;
    fp = fopen(label, "r+");
	
	SuperBlock* superBlock = getSuperBlock(fp);
	FileBlock* fileBlock = getFileBlock(fp, superBlock);
	AvailableBlock* availableBlock = getAvailableBlock(fp, superBlock);
	RefBlock* refBlock = getRefBlock(fp, superBlock);
	
	//Find the file to delete
	int idFile = searchFile(fileBlock, fileName, superBlock);
	
	if(idFile >= 0){
		fileBlock[idFile].available = 0;
		int idRef = fileBlock[idFile].firstBlock;
		
		uint i;
		
		//Look for all reference to free all block
		for(i=0;i<fileBlock[idFile].nbrBlock;i++){
			refBlock[idRef].available = 0;
			availableBlock[refBlock[idRef].dataBlock].available = 0;
			idRef = refBlock[idRef].refBlock;
		}
		
		setAvailableBlock(fp, superBlock, availableBlock);
		setFileBlock(fp, superBlock, fileBlock);
		setRefBlock(fp, superBlock, refBlock);
	}else{
		printf("This file doesn't exist \n");
	}
	
	return;
}

/*
* List of all file
* char* label name of the fs file
*/
void list(char* label){
	FILE *fp;
    fp = fopen(label, "r+");
	
	SuperBlock* superBlock = getSuperBlock(fp);
	FileBlock* fileBlock = getFileBlock(fp, superBlock);
	
	uint i;
	
	for(i=0;i<getNumberFile(superBlock);i++){
			if(fileBlock[i].available == 1)
				printf("%s %i B\n", fileBlock[i].name, fileBlock[i].nbrBlock*superBlock->blockSize);
	}
	
	free(superBlock);
	free(fileBlock);
	fclose(fp);
	
	return;
}

/*
* Info about the fs
* char* label name of the fs file
*/
void info(char* label){
    FILE *fp;
    fp = fopen(label, "r+");
    
    SuperBlock* superBlock = getSuperBlock(fp);
    AvailableBlock* availableBlock = getAvailableBlock(fp,superBlock);
    
    uint i;
    uint count = 0;
    
    for(i=0;i<superBlock->blockSize*superBlock->availableBlocksSize;i++){
         if(availableBlock[i].available == 1)
            count++;       
    }
    
    printf("Type: %i\n", superBlock->type);
    printf("Version: %i\n",superBlock->version);
    printf("Label: %s\n",superBlock->label);
    printf("Space Use: %i B\n",count*superBlock->blockSize);
    printf("Space Max: %i B\n", superBlock->blockSize*superBlock->multiBlock);
	
	free(superBlock);
	free(availableBlock);
	fclose(fp);
        
    return;
}

