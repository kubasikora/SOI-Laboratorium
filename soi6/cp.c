#include<stdio.h>
#include<string.h>

#include"fs.h"

int cp(char* name){
    FILE *fp, *cp_src;
    char fs_buffer[FS_SIZE];
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

    int maxSize = fs_sb.blockSize*fs_sb.dataBlockNum;
    char sourcebuffer[maxSize];

    //check if file with this name already exists
    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(iNodeBitmap[i] != '\0'){ 
            if(strcmp(iNodeTable[i].name, name) == 0){
                printf("File already exists!\n");
                return FILEALREADYEXISTS;
            }
        } 
    }

    //load file
    cp_src = fopen(name, "r");
    if(!fp) {
        printf("Target file do not exist!\n");
        return TARGETFILENOTEXISTING;
    }
    fread(sourcebuffer, maxSize, 1, cp_src);
    int fileSize = strlen(sourcebuffer);
    fclose(cp_src);

    int requiredBlocks = 1 + (fileSize / fs_sb.blockSize); //get new file size

    for(int i = 0; i < fs_sb.iNodesNum; i++){
        if(iNodeBitmap[i] == '\0'){
            for(int j = 0; j < fs_sb.dataBlockNum; j++){
                if(dataBitmap[j] == '\0'){
                    //save content here
                    int notAvailable = 0;
                    //find continous block of space
                    for(int k = 0; k < requiredBlocks; k++){
                        if(dataBitmap[j + k] == '\0'){
                            continue;
                        }
                        notAvailable = 1;
                    }

                    //if its not try again
                    if(notAvailable){
                        continue;
                    }

                    //save iNode here
                    iNodeTable[i].isValid = 0xFF;
                    iNodeTable[i].startingBlock = fs_sb.dataBlockOffset + j;
                    iNodeTable[i].accessRights = 0x07;
                    iNodeTable[i].size = fileSize;
                    time_t now;
                    time(&now);
                    iNodeTable[i].lastAccessed = now;
                    iNodeTable[i].lastModified = now;
                    strcpy(iNodeTable[i].name, name);
                    //iNode ready

                    fp = fopen(FS_NAME, "r+b");
                    if(!fp) {
                        printf("No file system existing!\n");
                        return NOFSYSTEMEXISTING;
                    }

                    //lock inode
                    fseek(fp, fs_sb.iNodeOccupancyBitmapOffset*fs_sb.blockSize + i*sizeof(char), SEEK_SET);
                    fwrite(&OCCUPIED, sizeof(char), 1, fp); 

                    //lock data block
                    for(int z = 0; z < requiredBlocks; z++){
                        fseek(fp, fs_sb.dataOccupancyBitmapOffset*fs_sb.blockSize + (j+z)*sizeof(char), SEEK_SET);
                        fwrite(&OCCUPIED, sizeof(char), 1, fp); 
                    }

                    //write iNode
                    fseek(fp, fs_sb.iNodesOffset*fs_sb.blockSize + i*fs_sb.iNodeSize, SEEK_SET);
                    fwrite(iNodeTable+i, fs_sb.iNodeSize, 1, fp);

                    //write data
                    fseek(fp, (fs_sb.dataBlockOffset+j)*fs_sb.blockSize, SEEK_SET);
                    fwrite(sourcebuffer, fileSize, 1, fp);

                    fflush(fp);
                    fclose(fp);
                    return 0;
                }    
            }
            
        }
    }
    return NOAVAILABLEINODES;
}