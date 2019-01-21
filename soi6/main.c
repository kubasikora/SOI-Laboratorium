#include<stdio.h>
#include<string.h>

#include"fs.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Please provide argument\n");
        return INSUFFICIENTARGS;
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
        if(argc < 3) {
            printf("Argument missing.\n");
            return INSUFFICIENTARGS;
        }
        return cp(argv[2]);
    }

    if(strcmp(argv[1], "rmfs") == 0){
        return rmfs();
    }

    if(strcmp(argv[1], "fsinfo") == 0){
        return fsinfo();
    }

    if(strcmp(argv[1], "touch") == 0){
        if(argc < 3) {
            printf("Argument missing.\n");
            return INSUFFICIENTARGS;
        }
        return touch(argv[2]);
    }

    printf("Incorrect argument.\n");
    return INCORRECTARGUMENT;
}