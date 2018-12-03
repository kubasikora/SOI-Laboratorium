#include<stdlib.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

#include"soi3.h"

static struct sembuf buf;

void decrease(int semid, int semnum){
    buf.sem_num = semnum;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    if(semop(semid, &buf, 1) == -1){
        perror("Podniesienie semafora nie powiodlo sie");
        exit(1);
    }
}

void increase(int semid, int semnum){
    buf.sem_num = semnum;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    if(semop(semid, &buf, 1) == -1){
        perror("Opuszczanie semafora nie powiodlo sie");
        exit(1);
    }
}