#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

#include"soi3.h"

int main(int argc, char *argv[]){
    int shmid, semid, shmid_pri, i, arg;
    int *buf, *buf_pri;

    arg = 10;
    if(argc > 1) arg = atoi(argv[1]);

    shmid = shmget(KEY, (MAX+2)*sizeof(int), IPC_CREAT|0600);
    if(shmid == -1){
        perror("Utworzenie segmentu pamieci wspoldzielonej bufora");
        exit(1);
    }
    buf = (int*)shmat(shmid, NULL, 0);
    if (buf == NULL){
        perror("Przylaczenie segmentu pamieci wspoldzielonej bufora");
        exit(1);
    }

    shmid_pri = shmget(KEY+1, (MAX+2)*sizeof(int), IPC_CREAT|0600);
    if(shmid_pri == -1){
        perror("Utworzenie segmentu pamieci wspoldzielonej bufora specjalnego");
        exit(1);
    }
    buf_pri = (int*)shmat(shmid_pri, NULL, 0);
    if (buf_pri == NULL){
        perror("Przylaczenie segmentu pamieci wspoldzielonej bufora specjalnego");
        exit(1);
    }
    
    semid = semget(KEY, 5, IPC_CREAT|IPC_EXCL|0600);
    if (semid == -1){
        semid = semget(KEY, 5, 0600);
        if (semid == -1){
            perror("Utworzenie tablicy semaforow");
            exit(1);
        }
    }
    else{
        indexZ = 0;
        indexO = 0;
        indexZpri = 0;
        indexOpri = 0;
        if (semctl(semid, 0, SETVAL, (int)MAX) == -1){ //ile pustych
            perror("Nadanie wartosci semaforowi 0");
            exit(1);
        }
        if (semctl(semid, 1, SETVAL, (int)0) == -1){ //ile pelnych
            perror("Nadanie wartosci semaforowi 1");
            exit(1);
        }

        if (semctl(semid, 2, SETVAL, (int)MAX) == -1){ //ile pustych specjalnych
            perror("Nadanie wartosci semaforowi 2");
            exit(1);
        }

        if (semctl(semid, 3, SETVAL, (int)0) == -1){ //ile pelnych specjalnych
            perror("Nadanie wartosci semaforowi 3");
            exit(1);
        }

        if (semctl(semid, 4, SETVAL, (int)1) == -1){ //tylko jeden proces w sekcji kryt
            perror("Nadanie wartosci semaforowi 4");
            exit(1);
        }
    }
    for (i=0; i<5; i++){
        decrease(semid, 0);
        decrease(semid, 4);
        buf[indexZ] = arg;
        indexZ = (indexZ+1)%MAX;
        increase(semid, 1);
        increase(semid, 4);
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        printf("%4d-%2d-%2d %2d:%2d:%2d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("%d. Wysłano: %d\n", i, arg);
    }
}