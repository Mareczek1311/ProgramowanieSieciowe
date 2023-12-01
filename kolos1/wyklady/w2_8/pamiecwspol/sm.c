#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>



int main(int argc, char* argv[]){

    int shmid;
    key_t key;
    char* data;
    
    if((key = ftok("sm.c", 'A')) == -1){
        perror("ftok");
        exit(1);
    }

    if((shmid = shmget(key, 1024, 0644 | IPC_CREAT)) == -1){
        perror("shmget");
        exit(1);
    }

    data = shmat(shmid, (void*) 0, 0);
    if(data == (char*) (-1)){
        perror("shmat");
        exit(1);
    }

    if(argc == 2){
        printf("zapisuje... \n");
        strncpy(data, argv[1], 1024);
        
    }else{
        printf("Czytam: %s \n", data);
    }

    if(shmdt(data) == -1){
        perror("shmdt");
        exit(1);
    }

    return 0;
}
