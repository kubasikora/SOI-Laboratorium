#include <stdio.h>
#include <stdlib.h>
#include <so1_lib.h>

int main(int argc, char* argv[]) {
	int arg, ret, pid;
	pid = getpid();
	
	if(argc > 1) {
		arg = atoi(argv[1]);
	}
	else {
		fprintf(stderr, "Error! No arguments given. \n");
		return -1;
	}
	
	ret = add_nr_to_pid(arg);
	printf("arg = %d, pid = %d, result = %d\n", arg, pid, ret);
	return 0;
}
	
