#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main(int argc, char* argv[]){

    key_t key;
    int shmid;
    char* data;
    
    if(argc>2){
        fprintf(stderr, "./a.out [dane] \n");
        exit(1);
    }

    if((key = ftok("sysv_sharedmem.c", 'R')) == -1){
        perror("ftok");
        exit(1);
    }

    if((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1){
        perror("shmget");
        exit(1);
    }

    data = shmat(shmid, (void*)0, 0);

    if(data == (char*)(-1)){
        perror("shmat");
        exit(1);
    }

    if(argc == 2){
        printf("Pisze do segmentu: %s \n", argv[1]);
        strncpy(data, argv[1], SHM_SIZE);
    }else{
        printf("Segment zawierta: %s \n", data);
    }

    if(shmdt(data) == -1){
        perror("shmdt");
        exit(1);
    }

    return 0;
}
