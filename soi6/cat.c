#include<stdio.h>
#include<string.h>

#include"fs.h"

int cat(char* name){
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

    FS_iNode iNodeTable[fs_sb.iNodesNum];
    char iNodeBitmap[fs_sb.iNodesNum];

    fp = fopen(FS_NAME, "a+b");
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
        if(iNodeBitmap[i] != '\0'){ 
            if(strcmp(iNodeTable[i].name, name) == 0){
                // its our file
                int startingAddress = iNodeTable[i].startingBlock*fs_sb.blockSize;
                int size = iNodeTable[i].size;
                char fileBuffer[size+1];

                fp = fopen(FS_NAME, "a+b");
                if(!fp) {
                    printf("No file system existing!\n");
                    return NOFSYSTEMEXISTING;
                }

                //load file
                fseek(fp, startingAddress, SEEK_SET);
                fread(fileBuffer, sizeof(char), size, fp);

                fclose(fp);
                fileBuffer[size] = '\0';

                printf("%s", fileBuffer);
                printf("\n");
                return 0;
            }
        } 
    }
    printf("File doesnt exist\n");
    return FILENOTEXISTINGINFS;
}