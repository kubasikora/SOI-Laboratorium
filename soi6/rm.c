#include<stdio.h>
#include<string.h>

#include"fs.h"

int rm(char* name){
    FILE *fp, *cp_src;
    char* FREE = 0x00;
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

    int maxSize = fs_sb.blockSize*fs_sb.dataBlockNum;
    char sourcebuffer[maxSize];

    //check if file with this name already exists
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(iNodeBitmap[i] != '\0'){ 
            printf("%s\n", iNodeTable[i].name);
            if(strcmp(iNodeTable[i].name, name) == 0){
                printf("removing...\n");
                //lock inode

                int bitmapBlockStart = iNodeTable[i].startingBlock - fs_sb.dataBlockOffset;
                int bitmapBlockNum = 1 + iNodeTable[i].size / fs_sb.blockSize;
                

                fp = fopen(FS_NAME, "r+b");
                if(!fp) {
                    printf("No file system existing!\n");
                    return NOFSYSTEMEXISTING;
                }
                fseek(fp, fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize + i*sizeof(char), SEEK_SET);
                fwrite(&FREE, sizeof(char), 1, fp); 

                for(int z = 0; z < bitmapBlockNum; z++){
                    fseek(fp, fs_sb.dataOccupancyBitmapOffset*fs_sb.blockSize + (z+i)*sizeof(char), SEEK_SET);
                    fwrite(&FREE, sizeof(char), 1, fp); 
                }

                fflush(fp);
                fclose(fp);
                return 0;
            }
        } 
    }
    printf("Could not find file\n");
    return FILENOTEXISTINGINFS;
}