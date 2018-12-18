#include "/usr/include/lib.h"
#include "/usr/include/minix/type.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int arg, arg_max, ret;
    
    if(argc > 1) arg = atoi(argv[1]);
    else {
        fprintf(stderr, "Error! No arguments given. \n");
        return -1;
    }

    if(arg<0) {
        fprintf(stderr, "Error! Negative argument given. \n");
        return -1;
    } 

    arg_max = arg + 10;

    while(arg <= arg_max){
        input.mp_l2 = arg;
        ret = _syscall(MM, 70, &input);
        if(ret>=0) printf("pid = %d, index %d\n", arg, ret);
        else fprintf(stderr, "Error %d: No entry in proctable for pid = %d\n", errno, arg);
        ++arg;
    }
    return 0;
}