#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(){

    key_t key;
    int semid;
    union semun arg;

    if((key = ftok("init.c", 'J')) == -1){
        perror("ftok");
        exit(1);
    }

    if((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1){
        perror("semget");
        exit(1);
    }

    arg.val = 1;
    if(semctl(semid, 0, SETVAL, arg) == -1){
        perror("semctl");
        exit(1);
    }

    return 0;
}
