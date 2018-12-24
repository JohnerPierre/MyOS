#include <stdint.h>
#include <stdbool.h>

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct __attribute__ ((__packed__))
{
    uchar byte;
} DataBlock ;

typedef struct __attribute__ ((__packed__))
{
    uint8_t available;
} AvailableBlock ;

typedef struct __attribute__ ((__packed__))
{
	uint8_t available;
    //Name of file
    char name[51];
    //Number of block + array of block position
    uint32_t nbrBlock;
	//Size in octet
	uint32_t size;
    //Start of our file
    uint32_t firstBlock;
} FileBlock  ;

typedef struct __attribute__ ((__packed__)) 
{
    uint8_t available;
    uint32_t dataBlock;
	uint32_t refBlock;
    char padding[7];
} RefBlock ;

typedef struct __attribute__ ((__packed__))
{
    //Name of Disk
    char label[10];
    uint8_t type;
    uint8_t version;
    
    uint32_t blockSize;
    // Specifiy how many block in the disk (blockSize*multiBlock = size)
    uint32_t multiBlock;
    
    uint32_t availableBlocksSize;
    uint32_t fileBlocksSize;
    uint32_t refBlocksSize;
    
}  SuperBlock;

/*
* Create a new fs
* char* fileName name of the fs
* char* label of the fs
* uint32_t blockSize size of block
* uint32_t multiBlock number of block
*/
void create(char* fileName, char* label, uint32_t blockSize, uint32_t multiBlock);
/*
* Add a file
* char* fileName name of the file
* char* label name of the fs file
*/
void add(char* fileName, char* label);
/*
* Delete a file
* char* fileName name of the file
* char* label name of the fs file
*/
void delete(char* fileName, char* label);
/*
* List of all file
* char* label name of the fs file
*/
void list(char* label);
/*
* Info about the fs
* char* label name of the fs file
*/
void info(char* label);