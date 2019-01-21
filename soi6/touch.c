#include<stdio.h>
#include<string.h>
#include<time.h>

#include"fs.h"

int touch(char* name){
    const char OCCUPIED = 0xff;
    FILE* fp;
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

    //check if file with this name already exists
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(iNodeBitmap[i] != '\0'){ 
            if(strcmp(iNodeTable[i].name, name) == 0){
                printf("File already exists!\n");
                return FILEALREADYEXISTS;
            }
        } 
    }
    
    // find free iNode 
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(iNodeBitmap[i] == '\0'){
            

            //iNodeTable[i].startingBlock = fs_sb.dataBlockOffset + i;
            iNodeTable[i].isValid = 0xFF;
            iNodeTable[i].accessRights = 0x07;
            iNodeTable[i].size = 0;
            time_t now;
            time(&now);
            iNodeTable[i].lastAccessed = now;
            iNodeTable[i].lastModified = now;
            memcpy(iNodeTable[i].name, name, strlen(name));
            //fs_buffer[fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize + i] = 0xFF;           
            //memcpy(&fs_buffer[fs_sb.blockSize*fs_sb.iNodesOffset + i*fs_sb.iNodeSize],
            //    &iNode,
            //    sizeof(FS_iNode));
            fp = fopen(FS_NAME, "r+b");
            if(!fp) {
                printf("No file system existing!\n");
                return NOFSYSTEMEXISTING;
            }

            printf("Bitmap %p\n", fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize + i*sizeof(char));
            fseek(fp, fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize + i*sizeof(char), SEEK_SET);
            fwrite(&OCCUPIED, sizeof(char), 1, fp); 

            //write iNode
            printf("iNode %p\n", fs_sb.iNodesOffset*fs_sb.blockSize + i*fs_sb.iNodeSize);
            fseek(fp, fs_sb.iNodesOffset*fs_sb.blockSize + i*fs_sb.iNodeSize, SEEK_SET);
            fwrite(iNodeTable+i, fs_sb.iNodeSize, 1, fp);
            fflush(fp);

            fclose(fp);
            return 0;
        }
    }
    
    printf("No available iNodes to create a new file\n");
    return NOAVAILABLEINODES;
}