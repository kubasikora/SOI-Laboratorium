#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"fs.h"

int mkfs(){
    FILE* fp;
    fp = fopen(FS_NAME, "r");
    if(fp){
        printf("Filesystem already exists!\n");
        fclose(fp);
        return -1;
    }

    char nullSign = '\0';
    fp = fopen(FS_NAME, "a+b");
    for(int i = 0; i < FS_SIZE; i++){
        fwrite(&nullSign, sizeof(char), 1, fp);
    }
    fclose(fp);

    FS_Superblock fs_sb;
    fs_sb.blockSize = 4096;

    fs_sb.iNodesBlockNum = 5;
    fs_sb.iNodesNum = 80;
    fs_sb.iNodeSize = 128;
    fs_sb.iNodesOffset = 3;

    fs_sb.dataBlockNum = 56;
    fs_sb.dataBlockOffset = 8;

    fs_sb.iNodeOccupancyBitmapOffset = 1;
    fs_sb.dataOccupancyBitmapOffset = 2;

    fp = fopen(FS_NAME, "r+b");
    fwrite(&fs_sb, sizeof(FS_Superblock), 1, fp);
    fclose(fp);

    FS_iNodeOccupancyBitmap inodeOBitmap;
    for(int i = 0; i < FS_INODES; i++){
        inodeOBitmap.occupied[i] = '\0';
    }

    FS_dataOccupancyBitmap dataOBitmap;
    for(int i = 0; i < FS_DATA_BLOCKS; i++){
        dataOBitmap.occupied[i] = '\0';
    }

    char fs_buffer[FS_SIZE];
    fp = fopen(FS_NAME, "r+b");
    fread(fs_buffer, FS_SIZE, 1, fp);
    fclose(fp);

    memcpy(fs_buffer+(fs_sb.blockSize*fs_sb.iNodeOccupancyBitmapOffset),
        inodeOBitmap.occupied,
        sizeof(FS_iNodeOccupancyBitmap));

    memcpy(fs_buffer+(fs_sb.blockSize*fs_sb.dataOccupancyBitmapOffset),
        dataOBitmap.occupied,
        sizeof(FS_dataOccupancyBitmap));

    fp = fopen(FS_NAME, "wb");
    fwrite(fs_buffer, FS_SIZE, 1, fp);
    fclose(fp);

    return 0;
}