#include "../common/types.h"
#include "ide.h"
#include "library.h"
#include "../common/term.h"

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
    
}  superblock_t;

typedef struct __attribute__ ((__packed__))
{
	uint8_t available;
    //Name of file
    char name[51];
    //Number of block
    uint32_t nbrBlock;
	//Size in octet
	uint32_t size;
    //Start of our file
    uint32_t firstBlock;
} fileblock_t;

typedef struct __attribute__ ((__packed__)) 
{
    uint8_t available;
    uint32_t dataBlock;
	uint32_t refBlock;
    char padding[7];
} refblock_t ;


typedef struct __attribute__ ((__packed__))
{
    uchar byte;
} DataBlock ;

typedef struct __attribute__ ((__packed__))
{
    uint8_t available;
} AvailableBlock ;


typedef struct{
	uint32_t nextFile;
} file_iterator_t;

typedef struct{
	char name[51];
	uint32_t size;
} stat_t;

typedef struct{
    bool available;
    uint32_t offset;
    uint32_t nbrBlock;
	uint32_t size;
    uint32_t firstBlock;
} descriptor_t;

void initDescriptor();
int file_stat(char *filename, stat_t *stat);
int file_remove(char *filename);
bool file_exists(char *filename);
int file_open(char *filename);
int file_read(int fd, void *buf, uint count);
int file_seek(int fd, uint offset);
void file_close(int fd);
file_iterator_t file_iterator();
bool file_has_next(file_iterator_t *it);
void file_next(char *filename, file_iterator_t *it);