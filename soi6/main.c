#include<stdio.h>
#include<string.h>

#include"fs.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Please provide argument\n");
        return -1;
    } 

    if(strcmp(argv[1], "mkfs") == 0){
        return mkfs();
    }

    if(strcmp(argv[1], "ls") == 0){
        return ls();
    }

    if(strcmp(argv[1], "rm") == 0){
        return rm();
    }

    if(strcmp(argv[1], "cp") == 0){
        return cp();
    }

    if(strcmp(argv[1], "rmfs") == 0){
        return rmfs();
    }

    if(strcmp(argv[1], "fsinfo") == 0){
        return fsinfo();
    }

    printf("Show help here\n");

    return -1;
}