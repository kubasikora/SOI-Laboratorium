#include<stdio.h>

#include"fs.h"

int fsinfo(){
    FILE *fp, *cp_src;
    const char OCCUPIED = 0xff;
    
    FS_Superblock fs_sb;

    fp = fopen(FS_NAME, "r+b");
    if(!fp) {
        printf("No file system existing!\n");
        return NOFSYSTEMEXISTING;
    }
    fread(&fs_sb, sizeof(FS_Superblock), 1, fp);
    fclose(fp);

    char iNodeBitmap[fs_sb.iNodesNum];
    char dataBitmap[fs_sb.dataBlockNum];
    FS_iNode iNodeTable[fs_sb.iNodesNum];

    fp = fopen(FS_NAME, "a+b");
    if(!fp) {
        printf("No file system existing!\n");
        return NOFSYSTEMEXISTING;
    }
    //load inode bitmap
    fseek(fp, fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize, SEEK_SET);
    fread(iNodeBitmap, sizeof(char), fs_sb.iNodesNum, fp);

    //load data bitmap
    fseek(fp, fs_sb.dataOccupancyBitmapOffset*fs_sb.blockSize, SEEK_SET);
    fread(dataBitmap, sizeof(char), fs_sb.dataBlockNum, fp);

    //load inode table
    fseek(fp, fs_sb.iNodesOffset*fs_sb.blockSize, SEEK_SET);
    fread(iNodeTable, fs_sb.iNodeSize, fs_sb.iNodesNum, fp);

    fclose(fp);

    int iNodeCounter = 0;
    int realSpaceUsage = 0;
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(iNodeBitmap[i] != 0x00){
            ++iNodeCounter;
            realSpaceUsage += iNodeTable[i].size;
        }
    }

    printf("iNode table starts at 0x%x and its occupied in %.2f%%\n", fs_sb.iNodesOffset*fs_sb.blockSize, 100*(float)iNodeCounter/(float)fs_sb.iNodesNum);
    printf("File system stores %d %s\n", iNodeCounter, iNodeCounter == 1 ? "file" : "files");
    printf("iNode bitmap: \n");
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        printf("|%c", iNodeBitmap[i] == 0x00 ? 'O' : 'X');
    }
    printf("|\n");

    int dataBlockCounter = 0;
    for(int i = 0; i < fs_sb.dataBlockNum; i++){
        if(dataBitmap[i] != 0x00) ++dataBlockCounter;
    }

    printf("Data space starts at 0x%x and its occupied in %.2f%%\n", fs_sb.dataBlockOffset*fs_sb.blockSize, 100*(float)dataBlockCounter/(float)fs_sb.dataBlockNum);
    printf("Real data space usage is equal %.2f%% (%d bytes)\n", 100*(float)realSpaceUsage/((float)fs_sb.dataBlockNum*fs_sb.blockSize), realSpaceUsage);
    printf("Data bitmap: \n");
    for(int i = 0; i < fs_sb.dataBlockNum; i++){
        printf("|%c", dataBitmap[i] == 0x00 ? 'O' : 'X');
    }
    printf("|\n");

    return 0;
}