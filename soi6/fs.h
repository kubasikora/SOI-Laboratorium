#ifndef __SOI_FS_H__
#define __SOI_FS_H__

#define FS_NAME "vsfs"
#define FS_SIZE 4096

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
    
} FS_iNodeOccupancyBitmap; 

#endif //__SOI_FS_H__
