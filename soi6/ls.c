#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#include"fs.h"

int ls(){
    FILE* fp;
    char fs_buffer[FS_SIZE];
    FS_Superblock fs_sb;

    fp = fopen(FS_NAME, "r+b");
    if(!fp) {
        printf("No file system existing!\n");
        return NOFSYSTEMEXISTING;
    }
    fread(&fs_sb, sizeof(FS_Superblock), 1, fp);
    fclose(fp);

    char iNodeBitmap[fs_sb.iNodesNum];
    FS_iNode iNodeTable[fs_sb.iNodesNum];

    fp = fopen(FS_NAME, "r+b");
    if(!fp) {
        printf("No file system existing!\n");
        return NOFSYSTEMEXISTING;
    }
    //load inode bitmap
    fseek(fp, fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize, SEEK_SET);
    fread(iNodeBitmap, sizeof(char), fs_sb.iNodesNum, fp);

    //load inode table
    fseek(fp, fs_sb.iNodesOffset*fs_sb.blockSize, SEEK_SET);
    fread(iNodeTable, fs_sb.iNodeSize, fs_sb.iNodesNum, fp);

    fclose(fp);
    
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(iNodeBitmap[i] != 0x00){
            printf("%s\n", iNodeTable[i].name);
        }
    }

    return 0;
}