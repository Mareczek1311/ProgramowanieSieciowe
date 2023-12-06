#include "queue.h"

void handler(int signal){
    if(msgctl() == -1){
        perror("msgctl RMID");
        exit(1);
    }

    printf("[Serwer] Kolejka zostala usunieta, serwer konczy prace \n");
}

int main(){

    signal(SIGINT, handler);

    struct my_msgbuf buf;
    int msqid;
    key_t key;

    if((key = ftok("queue.h", 'A')) == -1){
        perror("ftok");
        exit(1);
    }

    if((msqid = msgget(key, 0644 | IPC_CREAT | IPC_EXCL)) == -1){
        perror("msgget");
        exit(1);
    }

    printf("Czytam... \n");

    for(;;){
        if(msgrcv(msqid, (struct msgbuf*)&buf, sizeof(buf), 0, 0) == -1){
            perror("msgrcv");
            exit(1);
        }
    }

    return 0;
}
