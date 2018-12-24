#include "fs.h"
#define superblockBlocksSize 1
#define descriptorSize 50
#define sizeofname 51

descriptor_t descriptor[descriptorSize];

/*
* Initialisation of Descriptor Table
*/
void initDescriptor(){
    memset(descriptor,0,descriptorSize * sizeof(descriptor_t));
}

/*
* Get First Available Descriptor
* return IdDescriptor -1 if full else id
*/
int getFirstAvailableDescriptor(){
    for(uint i = 0; i < descriptorSize ;i++){
		if(descriptor[i].available == false)
		    return i;
	}
	return -1;
}

/*
* Get SuperBlock from img
* superblock_t *superblock
*/
int getSuperblock(superblock_t *superblock){
	uchar data[SECTOR_SIZE];
	read_sector(0,data);
	memcpy(superblock, data, sizeof(superblock_t));
	return 0;
}

/*
* Search file
* char *filename
* fileblock_t *fileblock
* superblock_t superblock
* return IdBlock -1 if not exist
*/
int file_search(char *filename, fileblock_t *fileblock, superblock_t superblock){
	uchar data[SECTOR_SIZE];
	uint idBlock = superblock.availableBlocksSize + superblockBlocksSize;
    int counter = 0;

	for(uint i = idBlock; i < (idBlock+superblock.fileBlocksSize);i++){
		read_sector(i,data);
		for(uint y = 0; y < SECTOR_SIZE; y += sizeof(fileblock_t)){
				memcpy(fileblock, &data[y], sizeof(fileblock_t));
				if(fileblock->available == 1 && strncmp(fileblock->name, filename, strlen(fileblock->name)) == 0){
					return counter;
				}	
				counter += sizeof(fileblock_t);
		}
	}
	return -1;
}

/*
* File Stat
* char *filename
* stat_t *stat exit value
* return Flag 0 if OK -1 if not exist
*/
int file_stat(char *filename, stat_t *stat){
	superblock_t superblock;
	fileblock_t fileblock;

	getSuperblock(&superblock);
	
	if(file_search(filename, &fileblock, superblock) >= 0){
		memcpy(stat->name, fileblock.name, sizeofname);
		stat->size = superblock.blockSize * fileblock.nbrBlock;
		return 0;
	}
	return -1;
}

/*
* File Remove
* char *filename
* return Flag 0 if OK -1 if not exist
*/
int file_remove(char *filename){
	uchar data[SECTOR_SIZE];
    superblock_t superblock;
	fileblock_t fileblock;

	getSuperblock(&superblock);
    
    int i = file_search(filename, &fileblock, superblock);
    if(i>=0){
        int id = superblock.availableBlocksSize + superblockBlocksSize  + i/superblock.blockSize;
        int offset = i%superblock.blockSize;
		read_sector(id,data);
        fileblock.available = 0;
		memcpy(&data[offset], &fileblock, sizeof(fileblock_t));
        write_sector(id,data);
        return 0;
    }
    
	return -1;
}

/*
* File Exist
* char *filename
* return Flag True if OK False if not exist
*/
bool file_exists(char *filename){
	superblock_t superblock;
	fileblock_t fileblock;

	getSuperblock(&superblock);
	
	if(file_search(filename, &fileblock, superblock) >= 0){
	    return true;
	}
	return false;
}

/*
* File Open
* char *filename
* return idDescriptor -q if not exist
*/
int file_open(char *filename){
	superblock_t superblock;
	fileblock_t fileblock;

	getSuperblock(&superblock);
	
	if(file_search(filename, &fileblock, superblock) >= 0){
	    int idDescriptor = getFirstAvailableDescriptor();
	    if(idDescriptor >= 0){
    	    descriptor[idDescriptor].available = true;
    	    descriptor[idDescriptor].offset = 0;
    	    descriptor[idDescriptor].nbrBlock = fileblock.nbrBlock;
    	    descriptor[idDescriptor].size = fileblock.size;
    	    descriptor[idDescriptor].firstBlock = fileblock.firstBlock;
	    }
	    return idDescriptor;
	}
	return -1;
}

/*
* Read File
* int fd link to the descriptor
* void* buf output
* uint count number of byte to read 
* return int number of byte read
*/
int file_read(int fd, void *buf, uint count){
    uchar data[SECTOR_SIZE];
    superblock_t superblock;
	getSuperblock(&superblock);
	
    if(descriptor[fd].offset == descriptor[fd].size)
        return 0;
    
    //Ref block
    refblock_t ref;
    //Number of data to read
    int counter = 0;
    //Check for last
    if(descriptor[fd].offset + count > descriptor[fd].size)
        counter = descriptor[fd].size - descriptor[fd].offset;
    else
        counter = count;
    
    //position of the offset in ref   
    uint offset = descriptor[fd].offset % superblock.blockSize;
    uint id = descriptor[fd].offset / superblock.blockSize;
    uint pos = 0;
    uint i = 0;
    
    //Set next ref at first position
    int nextRef = descriptor[fd].firstBlock;
    
    while(i < descriptor[fd].nbrBlock && counter > 0){
        //OffsetRef and idRef for next ref block
        uint offsetRef = nextRef % (superblock.blockSize/sizeof(refblock_t));
        uint idRef = nextRef / (superblock.blockSize/sizeof(refblock_t));
        //Read block
        read_sector(idRef + superblock.availableBlocksSize + superblock.fileBlocksSize + superblockBlocksSize,data);
        memcpy(&ref,&data[offsetRef*sizeof(refblock_t)],sizeof(refblock_t));
        nextRef = ref.refBlock;
        
        //start to read file at good block position
        if(i >= id){
            //Open block to read 
            read_sector(ref.dataBlock + superblock.availableBlocksSize + superblock.fileBlocksSize + superblock.refBlocksSize + superblockBlocksSize,data);
            if(counter >=  (int)superblock.blockSize){
                memcpy(buf + pos, &data[offset], counter);
                pos += (superblock.blockSize - offset);
            } 
            else{
                memcpy(buf + pos, &data[offset], counter);
                pos += counter;
            }
            counter -= (superblock.blockSize - offset);
            offset = 0;
        }
        i++;
    }
    
    file_seek(fd, descriptor[fd].offset + pos);
    return pos;
}

/*
* File Seek
* int fd
* uint offset
* return Flag 0 if OK else -1
*/
int file_seek(int fd, uint offset){
    if(offset <= descriptor[fd].size){
        descriptor[fd].offset = offset;
        return 0;
    }
	return -1;
}

/*
* File Close
* int fd id of descriptor
*/
void file_close(int fd){
    descriptor[fd].available = false;
	return;
}

/*
* Next File
* int id
* int offset
* superblock_t superblock
* return int nextfile
*/
int nextFile(int id, int offset, superblock_t superblock){
    uchar data[SECTOR_SIZE];
	fileblock_t fileblock;
    
	uint idBlock = superblock.availableBlocksSize + superblockBlocksSize;
	uint i;
	uint y = offset;
    
    int counter = offset + id * superblock.blockSize;
    
	for(i= idBlock + id; i < (idBlock+superblock.fileBlocksSize);i++){
		read_sector(i,data);
		while(y < SECTOR_SIZE){
			memcpy(&fileblock, &data[y], sizeof(fileblock_t));
			if(fileblock.available == 1){
				return counter;
			}	
		    y += sizeof(fileblock_t);
		    counter += sizeof(fileblock_t);
		}
		y = 0;
	}
	return counter;
}

/*
* Create File iterator
* return file_iterator_t
*/
file_iterator_t file_iterator(){
	file_iterator_t it;
    superblock_t superblock;
	getSuperblock(&superblock);
	it.nextFile = nextFile(0, 0, superblock);
	return it;
}

/*
* Iterator has next
* file_iterator_t it
* return bool true if next file
*/
bool file_has_next(file_iterator_t *it){
    uchar data[SECTOR_SIZE];
    superblock_t superblock;
	fileblock_t fileblock;
	getSuperblock(&superblock);

    uint id = it->nextFile/superblock.blockSize;
    if(id < superblock.fileBlocksSize){
        int offset = it->nextFile%superblock.blockSize;
    	int idBlock = superblock.availableBlocksSize + superblockBlocksSize + id;
    	
    	read_sector(idBlock,data);
    	memcpy(&fileblock, &data[offset], sizeof(fileblock_t));
    	
    	if(fileblock.available == 1){
    	    return true;
    	}
    }
	
	return false;
}

/*
* File Next
* file_iterator_t it
* char* filename output next name of file
*/
void file_next(char *filename, file_iterator_t *it){
    uchar data[SECTOR_SIZE];
    superblock_t superblock;
	fileblock_t fileblock;
	getSuperblock(&superblock);

    int id = it->nextFile/superblock.blockSize;
    int offset = it->nextFile%superblock.blockSize;
	int idBlock = superblock.availableBlocksSize + superblockBlocksSize + id;
	
	read_sector(idBlock,data);
	memcpy(&fileblock, &data[offset], sizeof(fileblock_t));
	memcpy(filename, fileblock.name, sizeofname);
	
	it->nextFile = nextFile(id, offset+=sizeof(fileblock_t), superblock);
	
	return;
}
