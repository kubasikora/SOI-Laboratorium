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
    fread(&fs_sb, sizeof(FS_Superblock), 1, fp);
    fclose(fp);

    fp = fopen(FS_NAME, "r+b");
    fread(fs_buffer, FS_SIZE, 1, fp);
    fclose(fp);
    
    FS_iNode iNode;
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(fs_buffer[fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize + i*fs_sb.iNodeSize] != '\0'){
            memcpy(&iNode, 
                &fs_buffer[fs_sb.blockSize*fs_sb.iNodesOffset + i*fs_sb.iNodeSize],
                sizeof(FS_iNode));
            printf("inode %d: %d\n", i, iNode.startingBlock);
        }
    }
    
    return 0;
}