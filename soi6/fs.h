#ifndef __SOI_FS_H__
#define __SOI_FS_H__

#include<time.h>

#define FS_NAME "vsfs"
#define FS_SIZE 262144
#define FS_INODES 80
#define FS_DATA_BLOCKS 56
#define FS_INODE_SIZE 128

int mkfs();
int ls();
int rm();
int cp();
int rmfs();
int fsinfo();

typedef struct FS_Superblock {
    int blockSize;
    
    int iNodeSize;
    int iNodesNum;
    int iNodesBlockNum;
    int iNodesOffset;

    int dataBlockNum; 
    int dataBlockOffset;

    int iNodeOccupancyBitmapOffset;
    int dataOccupancyBitmapOffset;
    
} FS_Superblock;

typedef struct FS_iNodeOccupancyBitmap {
    char occupied[FS_INODES];
} FS_iNodeOccupancyBitmap; 

typedef struct FS_dataOccupancyBitmap {
    char occupied[FS_DATA_BLOCKS];
} FS_dataOccupancyBitmap; 

typedef struct FS_iNode {
    int startingBlock;
    char accessRights;
    int size;
    time_t lastAccessed;
    time_t lastMofified;
    char name[96];
} FS_iNode;

#endif //__SOI_FS_H__
