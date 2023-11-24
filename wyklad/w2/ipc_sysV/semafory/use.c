#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(){

    key_t key;
    int semid;
    struct sembuf sb;
    sb.sem_op = 0;
    sb.sem_op = -1;
    char buf[1024];

    if((key = ftok("init.c", 'J')) == -1){
        perror("ftok");
        exit(1);
    }

    if((semid = semget(key, 1, 0)) == -1){
        perror("semget");
        exit(1);
    }

    printf("Press return to lock: \n");
    scanf("%s", buf);
    printf("Trying to lock...");
    
    if(semop(semid, &sb, 1) == -1){
        perror("semop");
        exit(1);
    }

    
    printf("Locked! \n");
    printf("Press return to unlock: \n");
    scanf("%s", buf);
      
    sb.sem_op = 1;

    if(semop(semid, &sb, 1) == -1){
        perror("semop");
        exit(1);
    }

    printf("Unlocked! \n");

    return 0;
}
