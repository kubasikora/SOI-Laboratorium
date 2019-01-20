#include<stdio.h>
#include<unistd.h>

#include"fs.h"

int rmfs(){
    FILE* fp;
    fp = fopen(FS_NAME, "r");
    if(fp){
        fclose(fp);
        unlink(FS_NAME);
        return 0;
    }
    printf("No filesystem exists!\n");
    return -1;
}